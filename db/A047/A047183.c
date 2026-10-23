/*
 * A047183  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(L, floor(n/3))} C(L, k) * C(n - L, 2k)
 *            where L = floor((n - 2) / 2)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A047183: Number of nonempty subsets of {1,…,n}
   in which exactly 1/3 of the elements are <= (n-2)/2. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small n explicitly to avoid unsigned underflow */
    if (n <= 3)
        return (seq_t)0;

    /* Compute L = floor((n-2)/2) safely */
    idx_t L = (n - 2) / 2;
    idx_t N2 = n - L;            /* Number of elements > L */

    /* k must satisfy 3k <= n and k <= L.  Additionally 2k must not exceed N2. */
    idx_t k_max = n / 3;
    if (k_max > L)
        k_max = L;

    seq_t sum = 0;
    seq_t binom_L_k = 1;    /* C(L,0) = 1 */
    seq_t binom_N2_r = 1;   /* C(N2,0) = 1 */

    for (idx_t k = 1; k <= k_max; ++k)
    {
        /* Update C(L, k) from C(L, k-1) */
        binom_L_k = (binom_L_k * (seq_t)(L - k + 1)) / (seq_t)k;

        /* r = 2k, previous r_prev = 2*(k-1) */
        idx_t r_prev = 2 * (k - 1);
        idx_t r = r_prev + 2;

        /* Break if we cannot choose 2k elements from N2 */
        if (r > N2)
            break;

        /* Update C(N2, r) from C(N2, r_prev) */
        seq_t numerator   = (seq_t)(N2 - r_prev) * (seq_t)(N2 - r_prev - 1);
        seq_t denominator = (seq_t)(r_prev + 1) * (seq_t)(r_prev + 2);
        binom_N2_r = (binom_N2_r * numerator) / denominator;

        sum += binom_L_k * binom_N2_r;
    }

    return sum;
}
