/*
 * A103244  Unreduced numerators of the elements T(n,k)/(n-k)!, read by rows,
 *          of the triangular matrix P^-1, which is the inverse of the matrix
 *          defined by P(n,k) = (-k^2-k)^(n-k)/(n-k)! for n >= k >= 1.
 * https://oeis.org/A103244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 28, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * Let T(n,k) be the unreduced numerators of the inverse of the
 * matrix P defined by P(n,k)=(-k^2-k)^(n-k)/(n-k)! for n>=k>=1.
 * Then T(k,k)=1 and for n>k
 *     T(n,k) = - Σ_{j=k+1}^{n} C(n-k,j-k)*(-k^2-k)^(j-k)*T(n,j).
 * The OEIS sequence a(n) is the flattened triangle of T(n,k)
 * read by rows :  a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), ...
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static idx_t max_row = 0;          /* biggest row that has been computed */
static seq_t **tri = NULL;         /* pointers to rows of T, indexed 1..max_row */

/* compute row 'i' of T and store it in tri[i] (1‑based indexing) */
static void compute_row(idx_t i)
{
    seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
    row[0] = 0;                   /* unused */
    row[i] = 1;                    /* base case T(i,i)=1 */

    for (idx_t k = i - 1; k >= 1; --k) {
        seq_t pos_sum = 0, neg_sum = 0;
        seq_t base_abs = (seq_t)(k * k + k);   /* | -k^2-k | */
        seq_t power = base_abs;                /* base^1 */
        seq_t binom = (seq_t)(i - k);          /* C(i-k,1) */
        idx_t exp = 1;                         /* current exponent = j-k */

        for (idx_t j = k + 1; j <= i; ++j) {
            seq_t term = binom * power;
            term = term * row[j];
            if ((exp & 1) == 0)              /* even exponent -> positive sign */
                pos_sum += term;
            else
                neg_sum += term;

            if (j < i) {                    /* advance to the next j */
                /* C(d, t+1) = C(d, t) * (d - t)/(t+1)  where d=i-k, t=exp */
                binom = binom * (seq_t)(i - k - exp) / (seq_t)(exp + 1);
                power = power * base_abs;
                ++exp;
            }
        }
        /* T(i,k) = neg_sum - pos_sum  (should be nonnegative) */
        seq_t val = neg_sum;
        if (pos_sum <= val)
            val -= pos_sum;
        else
            val = 0;                       /* safety: should not occur */
        row[k] = val;
    }
    tri[i] = row;
}

/* Compute the OEIS sequence A103244 */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    long double tmp = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((tmp - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (r - 1) * r / 2 >= n) --r;

    if (tri == NULL) {
        tri = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
        max_row = 0;
    }
    if (r > max_row) {
        size_t new_size = r + 1;
        tri = (seq_t **)realloc(tri, new_size * sizeof(seq_t *));
        for (idx_t i = max_row + 1; i <= r; ++i)
            compute_row(i);
        max_row = r;
    }

    idx_t prev_terms = r * (r - 1) / 2;
    idx_t k = n - prev_terms;          /* column index within row r */
    return tri[r][k];
}
