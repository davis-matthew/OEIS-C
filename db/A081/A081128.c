/*
 * A081128  12th binomial transform of (0,1,0,0,0,0,0,0,...).
 * https://oeis.org/A081128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 12^(n-1) (a(0) = 0, a(1) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two seq_t values. In the arbitrary‑precision build this
   uses the library’s multiplication operator, otherwise normal
   arithmetic is used.  */
static seq_t mul_seq_t(seq_t x, seq_t y) { return x * y; }

/* Fast binary exponentiation for base 12.  exp is unsigned, so it
   may be 0. */
static seq_t pow12(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 12;
    while (exp)
    {
        if (exp & 1) result = mul_seq_t(result, base);
        base = mul_seq_t(base, base);
        exp >>= 1;
    }
    return result;
}

/* Compute A081128: a(n) = n * 12^(n-1) for n >= 0.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */
    if (n == 1) return 1;           /* a(1) = 1 */
    seq_t p = pow12(n - 1);         /* 12^(n-1) */
    return mul_seq_t((seq_t)n, p);  /* n * 12^(n-1) */
}
