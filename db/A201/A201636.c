/*
 * A201636  Triangle read by rows, n>=0, k>=0, T(0,0) = 1, T(n,k) =
 *          Sum_{j=0..k} (C(n+k,k-j)*(-1)^(k-j)*2^(n-j)*Sum_{i=0..j}
 *          (C(n+j,i)*|S(n+j-i,j-i)|)), S = Stirling number of first kind.
 * https://oeis.org/A201636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   define T(n,k) by T(n,k) = Σ_{j=0..k} C(n+k, k-j) * (-1)^(k-j) *
 *            2^(n-j) * Σ_{i=0..j} C(n+j, i) * |S(n+j-i, j-i)| a(n) = T(r,k)
 *            where r = floor((sqrt(8n+1)-1)/2), k = n - r*(r+1)/2
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities required:
 *   seq_t binomial(idx_t a, idx_t b);
 *   seq_t pow2(idx_t exp);    // 2^exp
 */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* ----- Stirling number of the first kind (unsigned) caching ----- */
static seq_t *st_table = NULL; /* flat array of size (limit+1)^2 */
static int st_limit = -1;      /* current maximum n for which the table is valid */

/* Rebuild the Stirling table up to max_n (inclusive).  Complexity O(max_n^2). */
static void rebuild_stirling(int max_n)
{
    if (max_n <= st_limit) return;

    /* free previous table and allocate new one */
    if (st_table) free(st_table);
    int stride = max_n + 1;
    st_table = (seq_t*)calloc((size_t)stride * stride, sizeof(seq_t));
    if (!st_table) abort();   /* unlikely */

    for (int n = 0; n <= max_n; ++n) {
        /* n==0 */
        if (n == 0) {
            st_table[0] = 1;
            continue;
        }
        /* k=0 -> 0 for n>0 */
        st_table[n * stride + 0] = 0;
        /* diagonal */
        st_table[n * stride + n] = 1;
        /* fill interior */
        for (int k = 1; k < n; ++k) {
            seq_t left  = st_table[(n - 1) * stride + (k - 1)];
            seq_t mid   = st_table[(n - 1) * stride + k];
            seq_t mult  = mid * (seq_t)(n - 1);
            st_table[n * stride + k] = left + mult;
        }
    }
    st_limit = max_n;
}

/* Helper to get |S(n,k)| from cached table */
static seq_t stirling(int n, int k)
{
    int stride = st_limit + 1;
    return st_table[n * stride + k];
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* compute row r and column k (0‑based) */
    long double d = sqrtl((long double)(2.0L * (long double)n + 1.0L));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;

    /* maximum n+j in the formula is r + r = 2*r, so rebuild table accordingly */
    int max_n = (int)(r + r);
    rebuild_stirling(max_n);

    /* compute T(r,k) */
    seq_t pos = 0;   /* sum of terms with even sign */
    seq_t neg = 0;   /* sum of terms with odd sign */

    for (idx_t j = 0; j <= k; ++j) {
        /* compute inner sum Σ_{i=0..j} C(r+j, i) * |S(r+j-i, j-i)| */
        seq_t inner = 0;
        idx_t nj = r + j;          /* r + j for convenience */

        for (idx_t i = 0; i <= j; ++i) {
            idx_t idx_s = nj - i;          /* n + j - i  */
            idx_t off_s = j - i;           /* j - i      */
            seq_t binom = binomial(nj, i);
            seq_t s_val = stirling((int)idx_s, (int)off_s);
            inner += binom * s_val;
        }

        seq_t pow2_val = pow2((idx_t)(r - j));
        seq_t bin2 = binomial((idx_t)(r + k), (idx_t)(k - j));
        seq_t term = bin2 * pow2_val * inner;

        if (((k - j) & 1ULL) == 0) {
            pos += term;
        } else {
            neg += term;
        }
    }

    /* result is non‑negative; pos >= neg for all valid indices */
    return pos - neg;
}
