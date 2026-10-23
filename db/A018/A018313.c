/*
 * A018313  Divisors of 165.
 * https://oeis.org/A018313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 165 in ascending order */
/* a(1)=1, a(2)=3, a(3)=5, a(4)=11, a(5)=15, a(6)=33, a(7)=55, a(8)=165 */
/* For n<1 or n>8 the function returns 0. */

#include <stddef.h>

static const seq_t divs[8] = {
    (seq_t)1,
    (seq_t)3,
    (seq_t)5,
    (seq_t)11,
    (seq_t)15,
    (seq_t)33,
    (seq_t)55,
    (seq_t)165
};

seq_t a(idx_t n)
{
    if (n < (idx_t)1 || n > (idx_t)8)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
