/*
 * A018394  Divisors of 325.
 * https://oeis.org/A018394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 325 in increasing order, computed by enumerating
   its prime powers 5^a * 13^b for a = 0..2, b = 0..1. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for indices below the offset */

    /* There are exactly six divisors of 325.  Compute them once.
       325 = 5^2 * 13^1, so a = 0..2 and b = 0..1. */
    static seq_t divisors[6];
    static bool initialized = false;
    if (!initialized) {
        idx_t count = 0;
        for (int a = 0; a <= 2; ++a) {
            for (int b = 0; b <= 1; ++b) {
                seq_t val = 1;
                for (int i = 0; i < a; ++i)
                    val *= 5;
                for (int i = 0; i < b; ++i)
                    val *= 13;
                divisors[count++] = val;
            }
        }

        /* Sort the six values (bubble/insertion sort, trivial for 6 items) */
        for (idx_t i = 1; i < count; ++i) {
            seq_t key = divisors[i];
            idx_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n <= 6)
        return divisors[n - 1];
    return 0; /* no value beyond the last divisor */
}
