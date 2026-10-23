/*
 * A018361  Divisors of 261.
 * https://oeis.org/A018361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 261 in ascending order
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined by the build. */
#include <stdint.h>   /* For uint64_t used in intermediate computations */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* Divisors of 261 = 3^2 * 29^1 : 1,3,9,29,87,261 */
    static seq_t divisors[6];
    static int ready = 0;          /* flag to compute once */

    if (!ready) {
        /* Precompute powers of 3 and 29 */
        seq_t pow3[3];
        pow3[0] = 1;
        pow3[1] = 3;
        pow3[2] = 9;

        seq_t pow29[2];
        pow29[0] = 1;
        pow29[1] = 29;

        /* Generate all products in any order */
        int idx = 0;
        for (int e3 = 0; e3 < 3; ++e3) {
            for (int e29 = 0; e29 < 2; ++e29) {
                divisors[idx++] = pow3[e3] * pow29[e29];
            }
        }

        /* Simple insertion sort to order ascending */
        for (int i = 0; i < 5; ++i) {
            for (int j = i + 1; j < 6; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        ready = 1;
    }

    if (n <= 6)
        return divisors[n - 1];
    else
        return 0;   /* undefined beyond the last divisor */
}
