/*
 * A048999  Triangle giving coefficients of (n+1)!*B_n(x), where B_n(x) is a
 *          Bernoulli polynomial, ordered by falling powers of x.
 * https://oeis.org/A048999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, tabl
 * Formula:   a(n) is the nth coefficient of the triangle (n+1)!·B_n(x)
 *            defined by a(n,m) = (n+1)!·binom(n,m)·B_m, where 0 ≤ m ≤ n and
 *            the linear index n is n = max{k : k(k+1)/2 ≤ idx} m = idx -
 *            n(n+1)/2
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 66, i64 n <= 162,
 *            i128 n <= 419, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build; do not define them here. */

/* ---------- small utilities ---------- */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    if (a < (seq_t)0) a = -a;
    if (b < (seq_t)0) b = -b;
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static void reduce_rational(seq_t *num, seq_t *den)
{
    if (*den < (seq_t)0) {
        *den = -*den;
        *num = -*num;
    }
    seq_t g = gcd_seq(*num, *den);
    if (g > (seq_t)0) {
        *num /= g;
        *den /= g;
    }
}

/* ---------- factorial cache ---------- */

static seq_t *fact = NULL;      /* fact[i] = i!  */
static idx_t fact_size = 0;     /* number of stored factorials = size of array */

static void ensure_factorials(idx_t n)
{
    if (fact_size > n) return;          /* already computed */
    seq_t *tmp = (seq_t *)realloc(fact, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) return;                   /* allocation failure: caller will fail */
    if (fact_size == 0) {
        tmp[0] = (seq_t)1;
        fact_size = 1;
    }
    for (idx_t i = fact_size; i <= n; ++i) {
        tmp[i] = tmp[i - 1] * (seq_t)i;
    }
    fact = tmp;
    fact_size = n + 1;
}

/* ---------- Bernoulli number cache ---------- */

static seq_t *Bnum = NULL;   /* numerator of B_k   */
static seq_t *Bden = NULL;   /* denominator of B_k */
static idx_t B_size = 0;     /* number of stored B_k = size of arrays */

static void ensure_bernoulli(idx_t m)
{
    if (B_size > m) return;   /* already computed */

    /* Resize arrays if necessary */
    if (B_size == 0) {
        Bnum = (seq_t *)malloc(sizeof(seq_t));
        Bden = (seq_t *)malloc(sizeof(seq_t));
        if (!Bnum || !Bden) return;
        Bnum[0] = (seq_t)1;
        Bden[0] = (seq_t)1;
        B_size = 1;
    } else if (B_size <= m) {
        seq_t *tmp_num = (seq_t *)realloc(Bnum, (size_t)(m + 1) * sizeof(seq_t));
        seq_t *tmp_den = (seq_t *)realloc(Bden, (size_t)(m + 1) * sizeof(seq_t));
        if (!tmp_num || !tmp_den) return;
        Bnum = tmp_num;
        Bden = tmp_den;
    }

    /* Compute Bernoulli numbers B_k for k = B_size .. m */
    for (idx_t k = B_size; k <= m; ++k) {
        /* sum_{j=0}^{k-1} binom(k+1,j) * B_j */
        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;
        seq_t binom = (seq_t)1;   /* binom(k+1,0) */

        for (idx_t j = 0; j < k; ++j) {
            /* term = binom * B_j / Bden[j] */
            seq_t term_num = binom * Bnum[j];
            seq_t term_den = Bden[j];

            /* sum = sum + term */
            seq_t new_num = sum_num * term_den + term_num * sum_den;
            seq_t new_den = sum_den * term_den;
            sum_num = new_num;
            sum_den = new_den;
            reduce_rational(&sum_num, &sum_den);

            /* update binom to binom(k+1, j+1) */
            /* binom = binom * (k+1-j) / (j+1)  ; division is exact */
            seq_t mul = binom * (seq_t)(k + 1 - j);
            seq_t div = (seq_t)(j + 1);
            seq_t g = gcd_seq(mul, div);
            mul /= g;
            div /= g;
            binom = mul / div;   /* exact division */
        }

        /* B_k = - sum / (k+1)  */
        seq_t num = -sum_num;
        seq_t den = sum_den * (seq_t)(k + 1);
        reduce_rational(&num, &den);
        Bnum[k] = num;
        Bden[k] = den;
    }
    B_size = m + 1;
}

/* ---------- integer cubic root using binary search ---------- */
static idx_t triangular_row(idx_t idx)
{
    /* Find largest n such that n*(n+1)/2 <= idx */
    idx_t low = 0;
    idx_t high = sqrt((double)idx * 2.0) + 2; /* rough upper bound */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * (mid + 1) / 2 <= idx)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* map linear index n to (row, col) */
    idx_t row = triangular_row(n);
    idx_t col = n - row * (row + 1) / 2;

    /* precompute needed factorials and Bernoulli numbers */
    ensure_factorials(row + 1);
    ensure_bernoulli(col);

    /* compute binomial(row, col) = row! / (col! (row-col)!) */
    seq_t binom = fact[row];
    binom /= fact[col];
    binom /= fact[row - col];

    /* compute result = fact[row+1] * binom * Bnum[col] / Bden[col] */
    seq_t num = fact[row + 1] * binom;
    seq_t den = Bden[col];

    seq_t result = num * Bnum[col] / den;

    return result;
}
