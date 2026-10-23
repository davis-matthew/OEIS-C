/*
 * A008833  Largest square dividing n.
 * https://oeis.org/A008833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = ∏p∈P(n) p^(2⋅⌊v_p(n)/2⌋) (product of the largest square
 *            power of each prime factor of n)
 * Range:     u8 n <= 255, u16 n <= 9763, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Sequence is defined for n ≥ 1; return 0 for safety if n==0. */
    if (n == 0) return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Handle factor 2 separately. */
    if ((m & 1ULL) == 0) {
        int exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            exp++;
        }
        int even_exp = exp & ~1;                   /* largest even ≤ exp   */
        if (even_exp > 0) {
            seq_t pow2 = 1;
            for (int i = 0; i < even_exp; ++i)
                pow2 *= (seq_t)2;
            result *= pow2;
        }
    }

    /* Factor odd primes. */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            int even_exp = exp & ~1;
            if (even_exp > 0) {
                seq_t p_pow = 1;
                for (int i = 0; i < even_exp; ++i)
                    p_pow *= (seq_t)p;
                result *= p_pow;
            }
        }
    }

    /* If m > 1, it is a prime with exponent 1; contributes nothing. */

    return result;
}
