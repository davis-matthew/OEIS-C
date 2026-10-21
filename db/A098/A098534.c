/*
 * A098534  Mod 3 analog of Stern's diatomic series.
 * https://oeis.org/A098534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} C(n-k-1,k) modulo 3
 * Range:     u8 n <= 4374, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_mod3.h"
/* The result is accumulated as a plain integer sum of the residues.   */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t limit = (n - 1) / 2;          /* floor((n-1)/2) */
    seq_t sum = 0;
    for (idx_t k = 0; k <= limit; ++k) {
        idx_t N = n - k - 1;            /* N = n-k-1 >= k for k <= limit */
        int r = binom_mod3(N, k);
        sum += (seq_t)r;
    }
    return sum;
}
