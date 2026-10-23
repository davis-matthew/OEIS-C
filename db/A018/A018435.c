/*
 * A018435  Divisors of 404.
 * https://oeis.org/A018435
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

/* Divisors of 404 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool built = false;

    if (!built) {
        divisors[0] = (seq_t)1;
        divisors[1] = (seq_t)2;
        divisors[2] = (seq_t)4;
        divisors[3] = divisors[0] * (seq_t)101; /* 101 */
        divisors[4] = divisors[1] * (seq_t)101; /* 202 */
        divisors[5] = divisors[2] * (seq_t)101; /* 404 */
        built = true;
    }

    if (n == 0 || n > 6) return (seq_t)0;
    return divisors[n - 1];
}
