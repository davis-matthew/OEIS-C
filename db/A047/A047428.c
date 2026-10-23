/*
 * A047428  Numbers that are congruent to {0, 1, 3, 4, 5, 6} mod 8.
 * https://oeis.org/A047428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + [0,1,3,4,5,6][(n-1) mod 6]
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers congruent to {0,1,3,4,5,6} mod 8. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* offset 1: a(1)=0. For n<1 the behaviour is irrelevant. */
    if (n == 0) return (seq_t)0;   /* safety guard, not used */
    idx_t k = (n - 1) / 6;          /* block index */
    idx_t r = (n - 1) % 6;          /* position inside block */
    static const unsigned char map[6] = {0, 1, 3, 4, 5, 6};
    return ((seq_t)k * (seq_t)8 + map[r]);
}
