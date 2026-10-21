/*
 * A097377  a(n) = CubeFreeKernel(n) + 1.
 * https://oeis.org/A097377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = CubeFreeKernel(n) + 1
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the OEIS sequence A097377:
   a(n) = CubeFreeKernel(n) + 1,
   where CubeFreeKernel(n) = ∏ p^min(2, e_p) for n = ∏ p^e_p.   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1 */
    seq_t kernel = 1;
    uint64_t m = n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp == 1)
                kernel *= (seq_t)p;
            else
                kernel *= (seq_t)p * (seq_t)p;
        }
    }

    if (m > 1)                     /* remaining prime factor */
        kernel *= (seq_t)m;

    return kernel + 1;
}
