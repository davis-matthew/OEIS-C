/*
 * A105852  a(n) = sigma(n) mod 9.
 * https://oeis.org/A105852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) mod 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    int mod9 = 1;            /* product of σ(p^e) modulo 9 for all factors */

    /* factor 2 separately */
    if (m != 0) {
        int e = 0;
        while ((m & 1ULL) == 0ULL) {
            m >>= 1;
            ++e;
        }
        if (e > 0) {
            int sum = 1;              /* σ(2^e) modulo 9, start with 1 */
            int term = 1;             /* p^0 mod 9 */
            int pmod = 2 % 9;
            for (int i = 0; i < e; ++i) {
                term = (term * pmod) % 9;
                sum   = (sum + term) % 9;
            }
            mod9 = (mod9 * sum) % 9;
        }
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            int sum = 1;
            int term = 1;
            int pmod = (int)(p % 9);
            for (int i = 0; i < e; ++i) {
                term = (term * pmod) % 9;
                sum  = (sum + term) % 9;
            }
            mod9 = (mod9 * sum) % 9;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (m > 1) {
        int sum = 1;
        int term = 1;
        int pmod = (int)(m % 9);   /* exponent = 1 */
        term = (term * pmod) % 9;
        sum  = (sum + term) % 9;
        mod9 = (mod9 * sum) % 9;
    }

    return (seq_t)mod9;
}
