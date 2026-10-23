/*
 * A018627  Divisors of 752.
 * https://oeis.org/A018627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 752 – a(n) is the nth divisor in ascending order. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divs[10];
    static idx_t count = 0;

    if (!initialized) {
        /* Enumerate all positive divisors of 752 (16 * 47). */
        for (idx_t i = 1; i <= 752; ++i) {
            if (752 % i == 0) {
                divs[count++] = (seq_t)i;
                if (count == 10) break;   /* 752 has exactly ten divisors */
            }
        }
        initialized = true;
    }

    if (n == 0) return 0;           /* offset is 1, guard for accidental use of n=0 */
    if (n <= count) return divs[n - 1];
    return 0;                        /* undefined for n > 10 */
}
