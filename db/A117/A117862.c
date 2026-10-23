/*
 * A117862  Number of palindromes (in base 3) below 3^n.
 * https://oeis.org/A117862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*3^((n-1)/2)-2 for odd n; 2*3^(n/2)-2 for even n
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0 */

#include "../utils/pow3.h"    /* provides seq_t pow3(idx_t exp) */

seq_t a(idx_t n)
{
    /* 3^((n-1)/2) for odd n equals 3^(n/2) because C integer division truncates */
    seq_t p   = pow3(n / 2);              /* 3^(n/2) for even, floor((n-1)/2) for odd */
    seq_t res = p * ((n & 1) ? 4 : 2);   /* factor is 4 if n odd, else 2 */
    res -= 2;                            /* subtract 2 */
    return res;
}
