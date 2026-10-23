/*
 * A165560  The arithmetic derivative of n, modulo 2.
 * https://oeis.org/A165560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if the arithmetic derivative d(n) is odd, else 0. Using
 *            d(n) = n * Σ(k_i / p_i) for n = ∏ p_i^k_i. This reduces to: - if
 *            n is even and the exponent of 2 in n equals 1, a(n)=1; -
 *            otherwise, if n is odd, a(n) is the parity of the number of odd
 *            prime factors with odd exponent (XOR of expone...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

extern seq_t a(idx_t n)
{
    if (n <= 1) {       /* a(0)=0, a(1)=0 as given */
        return 0;
    }

    /* Count powers of two */
    idx_t m = n;
    unsigned cnt2 = 0;
    while ((m & 1ULL) == 0ULL) {   /* m % 2 == 0 */
        m >>= 1;                   /* m /= 2 */
        ++cnt2;
    }

    if (cnt2 == 1) {               /* n even and exponent of 2 is exactly 1 */
        return 1;
    }

    if (cnt2 != 0) {               /* n even and exponent of 2 ≠ 1 → derivative even */
        return 0;
    }

    /* n is odd now; compute parity of exponents of odd primes */
    bool parity = false;           /* XOR accumulator, bool behaves as 0/1 */

    idx_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            unsigned cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            if ((cnt & 1U) != 0) {  /* odd exponent contributes 1 */
                parity = !parity;
            }
        }
        p += 2;                     /* next odd candidate */
    }

    if (m > 1) {                    /* remaining prime factor with exponent 1 */
        parity = !parity;
    }

    return parity ? 1 : 0;
}
