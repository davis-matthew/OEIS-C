/*
 * A082296  Solutions to 13^x+17^x == 19 mod 23.
 * https://oeis.org/A082296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1-3*(-1)^n+22*n)/2
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = 11*n+1 if n is odd, 11*n-2 if n is even */

#include <stdint.h>               /* for uint64_t on some platforms */

seq_t a(idx_t n)
{
    /* for safety use a 64‑bit intermediate (n is uint64_t) */
    uint64_t val = 22 * (uint64_t)n;   /* 22*n  */
    val -= 1;                          /* 22*n - 1          */
    if (n & 1ULL)                      /* n is odd          */
        val += 3;                      /* (22*n - 1) + 3    */
    else
        val -= 3;                      /* (22*n - 1) - 3    */
    val /= 2;                          /* divide by 2       */
    return (seq_t)val;                 /* cast to sequence type */
}
