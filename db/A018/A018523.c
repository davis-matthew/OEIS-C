/*
 * A018523  Divisors of 568.
 * https://oeis.org/A018523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 568 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute the 8 divisors of 568 only once. */
    static seq_t divisors[8];
    static bool init = false;

    if (!init) {
        /* 568 = 2^3 * 71^1 → 8 divisors */
        const idx_t exp2  = 3;
        const idx_t exp71 = 1;
        size_t idx = 0;

        for (idx_t e2 = 0; e2 <= exp2; ++e2) {
            seq_t pow2 = (seq_t)1;
            for (idx_t i = 0; i < e2; ++i) pow2 *= (seq_t)2;

            for (idx_t e71 = 0; e71 <= exp71; ++e71) {
                seq_t pow71 = (seq_t)1;
                for (idx_t i = 0; i < e71; ++i) pow71 *= (seq_t)71;

                divisors[idx++] = pow2 * pow71;
            }
        }

        /* Sort the 8 elements (negligible cost). */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        init = true;
    }

    /* n is 1-based. Return 0 for out-of-range indices. */
    if (n == 0 || n > 8) return (seq_t)0;
    return divisors[n - 1];
}
