/*
 * A122212  a(n) is the n-th integer among the positive integers coprime to
 *          the sum of the divisors of n.
 * https://oeis.org/A122212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer coprime to σ(n)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* σ(n) is the sum of divisors of n */
    seq_t sigma = sigma_of(n);

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (gcd((seq_t)k, sigma) == 1) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
