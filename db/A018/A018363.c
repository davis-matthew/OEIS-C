/*
 * A018363  Divisors of 266.
 * https://oeis.org/A018363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 266 in ascending order; a(n) defined for 1 ≤ n ≤ 8 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t divisors[8] = {
        (seq_t)1,  (seq_t)2,  (seq_t)7,  (seq_t)14,
        (seq_t)19, (seq_t)38, (seq_t)133, (seq_t)266
    };
    if (n == 0 || n > 8) return (seq_t)0;
    return divisors[(size_t)(n - 1)];
}
