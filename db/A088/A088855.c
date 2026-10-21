/*
 * A088855  Triangle read by rows: number of symmetric Dyck paths of
 *          semilength n with k peaks.
 * https://oeis.org/A088855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 239, u32 n <= 721, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A088855 – triangle read by rows
   a(n) is the (n‑th) element of the flattened triangle
   T(r,k) = binom( floor((r-1)/2), floor((k-1)/2) ) *
            binom( ceil ((r-1)/2), ceil (k/2) )
   where row r (r ≥ 1) has r elements (k = 1…r) */
#include <stdint.h>

static seq_t binom(idx_t n, idx_t k) {
    /* Compute binomial coefficient C(n,k) using an
       iterative multiplicative formula to keep
       intermediates integral.  n and k fit in idx_t. */
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - i + 1); /* multiply by numerator */
        res = res / i;           /* divide by denominator */
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    /* Position k within the row (1‑based) */
    idx_t k = n - r * (r - 1) / 2;

    /* Parameters for the binomial formula */
    idx_t n1 = (r - 1) / 2;   /* floor((r-1)/2)  */
    idx_t n2 = r / 2;         /* ceil ((r-1)/2)  */
    idx_t k1 = (k - 1) / 2;   /* floor((k-1)/2)  */
    idx_t k2 = k / 2;         /* ceil (k/2)      */

    seq_t term1 = binom(n1, k1);
    seq_t term2 = binom(n2, k2);
    return term1 * term2;
}
