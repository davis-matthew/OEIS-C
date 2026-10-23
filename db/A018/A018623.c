/*
 * A018623  Divisors of 744.
 * https://oeis.org/A018623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest divisor of 744
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build.  The function has
   external linkage because a prototype already exists elsewhere. */
seq_t a(idx_t n)
{
    /* 744 = 2^3 * 3 * 31 has 16 divisors. */
    static seq_t divisors[16];
    static bool ready = false;
    if (!ready) {
        size_t idx = 0;
        for (unsigned i = 0; i <= 3; ++i) {
            seq_t pow2 = 1;
            for (unsigned a = 0; a < i; ++a) pow2 *= 2;
            for (unsigned j = 0; j <= 1; ++j) {
                seq_t pow3 = pow2;
                if (j) pow3 *= 3;
                for (unsigned k = 0; k <= 1; ++k) {
                    seq_t val = pow3;
                    if (k) val *= 31;
                    divisors[idx++] = val;
                }
            }
        }
        /* Sort the 16 values in ascending order. */
        for (size_t i = 0; i < 16; ++i) {
            for (size_t j = i + 1; j < 16; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        ready = true;
    }

    if (n < 1 || n > 16) return 0;   /* out of bounds – undefined behaviour not required */
    return divisors[(size_t)n - 1];
}
