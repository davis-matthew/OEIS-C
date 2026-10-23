/*
 * A010056  Characteristic function of Fibonacci numbers: a(n) = 1 if n is a
 *          Fibonacci number, otherwise 0.
 * https://oeis.org/A010056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is 1 if n is a Fibonacci number, 0 otherwise
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 8963, u64 n <= 8963,
 *            u128 n <= 8963, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return (s * s == x) ? (seq_t)1 : (seq_t)0;
}

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t tmp = n2 * (seq_t)5;    /* 5*n^2 */
    seq_t val1 = tmp + (seq_t)4;  /* 5*n^2 + 4 */
    if (is_square(val1)) return (seq_t)1;
    if (n > 0)
    {
        seq_t val2 = tmp - (seq_t)4; /* 5*n^2 - 4 */
        if (is_square(val2)) return (seq_t)1;
    }
    return (seq_t)0;
}
