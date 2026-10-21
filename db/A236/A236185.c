/*
 * A236185  Differences between terms of compacting Eratosthenes sieve for
 *          prime(4) = 7.
 * https://oeis.org/A236185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*((n+2) mod 2) + 2*((n+1) mod 2) + 4*(f(8,n+2)+f(8,n)) -
 *            2*f(8,n+1) where f(x,n)=⌊n/x⌋−⌊(n−1)/x⌋. Since f(8,n) = 1 iff n
 *            is divisible by 8, the sequence is 8‑periodic and the following
 *            implementation computes it directly for any n ≥ 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; safety for 0 */
        return (seq_t)0;

    /* n mod 8 for the f(8,·) terms */
    idx_t mod8 = n % 8;

    /* parity of n: 0 for even, 1 for odd */
    seq_t parity = n & 1;

    /* Base terms: 4*((n+2) mod 2) + 2*((n+1) mod 2) */
    seq_t result = 4 * parity + 2 * (parity ^ 1);

    /* f(8,·) terms: 1 when the argument is divisible by 8, else 0 */
    seq_t f_n     = (mod8 == 0)               ? 1 : 0;  /* f(8,n) */
    seq_t f_nplus1 = ((mod8 + 1) % 8 == 0)   ? 1 : 0;  /* f(8,n+1) */
    seq_t f_nplus2 = ((mod8 + 2) % 8 == 0)   ? 1 : 0;  /* f(8,n+2) */

    /* Add and subtract the f terms as per the formula */
    result += 4 * (f_n + f_nplus2);
    result -= 2 * f_nplus1;

    return result;
}
