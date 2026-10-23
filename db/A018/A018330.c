/*
 * A018330  Divisors of 196.
 * https://oeis.org/A018330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 196 in ascending order
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* There are only 9 divisors of 196 = 2^2 * 7^2 */
    static seq_t divisors[9];
    static bool initialized = false;

    if (!initialized) {
        /* Generate all divisors */
        int k = 0;
        for (int a = 0; a <= 2; ++a) {
            for (int b = 0; b <= 2; ++b) {
                seq_t d = 1;
                for (int i = 0; i < a; ++i) d *= 2;
                for (int i = 0; i < b; ++i) d *= 7;
                divisors[k++] = d;
            }
        }
        /* Simple insertion sort – works for the tiny array */
        for (int i = 1; i < 9; ++i) {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key) {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        initialized = true;
    }

    if (n < 1 || n > 9)
        return (seq_t)0;                /* undefined beyond the finite list */

    return divisors[(size_t)(n - 1)];
}
