/*
 * A126780  Number of functions f:{1,2,...,n}->{1,2,...,n} such that Im(f)
 *          contains 4 fixed elements.
 * https://oeis.org/A126780
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n-4*(n-1)^n+6*(n-2)^n-4*(n-3)^n+(n-4)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer power for seq_t using exponentiation by squaring.
   Assumes seq_t supports built-in arithmetic operators.        */
static seq_t ipow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;   /* undefined for n < 4, return 0 */

    seq_t p0 = ipow((seq_t)n, n);
    seq_t p1 = ipow((seq_t)(n - 1), n);
    seq_t p2 = ipow((seq_t)(n - 2), n);
    seq_t p3 = ipow((seq_t)(n - 3), n);
    seq_t p4 = ipow((seq_t)(n - 4), n);

    return p0 - ((seq_t)4 * p1) + ((seq_t)6 * p2) - ((seq_t)4 * p3) + p4;
}
