/*
 * A108400  a(n) = Product_{k = 0..n} (2^k * k!).
 * https://oeis.org/A108400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Product_{k = 0..n} (2^k * k!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t factorial = 1;   /* i! at each step */
    seq_t prod_fact = 1;   /* product_{k=0..i} k!  */
    seq_t pow2pow   = 1;   /* 2^{1+2+...+i}       */

    for (idx_t i = 1; i <= n; ++i)
    {
        factorial *= (seq_t)i;      /* factorial = i!   */
        prod_fact *= factorial;     /* prod_fact = prod_{k=0..i} k! */
        pow2pow <<= i;              /* multiply by 2^i  */
    }

    return prod_fact * pow2pow;    /* final product */
}
