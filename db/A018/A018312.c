/*
 * A018312  Divisors of 164.
 * https://oeis.org/A018312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 164 in ascending order, 0 if n>6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool initialized = false;
    if (!initialized) {
        /* compute all divisors of 164 */
        idx_t count = 0;
        const idx_t N = 164;
        for (idx_t i = 1; i <= N; ++i) {
            if (N % i == 0) {
                divisors[count++] = (seq_t)i;
                if (count == 6) break; /* we know there are exactly 6 divisors */
            }
        }
        initialized = true;
    }
    if (n >= 1 && n <= 6) {
        return divisors[(idx_t)n - 1];
    }
    return 0; /* undefined for n > 6 */
}
