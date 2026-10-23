/*
 * A168088  a(n) = 2^tetranacci(n).
 * https://oeis.org/A168088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^A000078(n)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000078(n) is the tetranacci number with initial values 0,0,0,1  */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute the nth tetranacci number */
    seq_t a0 = 0, a1 = 0, a2 = 0, a3 = 1;  /* a3 corresponds to n = 3 */

    if (n == 0) return (seq_t)1; /* 2^0 */
    if (n == 1) return (seq_t)1; /* 2^0 */
    if (n == 2) return (seq_t)1; /* 2^0 */
    if (n == 3) return (seq_t)2; /* 2^1 */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = a0 + a1 + a2 + a3;  /* a(n) = a(n-4)+a(n-3)+a(n-2)+a(n-1) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    seq_t exp = a3;                     /* exp = A000078(n) */
    return big_pow((seq_t)2, exp);      /* a(n) = 2^exp */
}
