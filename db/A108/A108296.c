/*
 * A108296  Diagonal sums of the number triangle associated to A086617.
 * https://oeis.org/A108296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} Sum_{j=0..n-2k} C(n-2k, j) * C(k, j) *
 *            Catalan(j)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are defined in the build environment.
    This implementation uses only integer arithmetic and no external utilities.    */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t nk = n - 2 * k;
        /* The inner sum runs only while j <= k; otherwise C(k, j)=0 */
        idx_t limit = nk < k ? nk : k;
        seq_t binom_n = 1;  /* C(nk, j) */
        seq_t binom_k = 1;  /* C(k, j) */
        seq_t cat     = 1;  /* Catalan(j) */
        for (idx_t j = 0; j <= limit; ++j) {
            if (j == 0) {
                /* For j = 0 we have binom_n = binom_k = cat = 1 */
                total += 1;
            } else {
                /* Update C(nk, j)   */
                binom_n *= (seq_t)(nk - j + 1);
                binom_n /= (seq_t)j;
                /* Update C(k, j)    */
                binom_k *= (seq_t)(k - j + 1);
                binom_k /= (seq_t)j;
                /* Update Catalan(j)  */
                cat *= (seq_t)(4 * j - 2);
                cat /= (seq_t)(j + 1);
                /* Add contribution of this (k, j) pair */
                total += binom_n * binom_k * cat;
            }
        }
    }
    return total;
}
