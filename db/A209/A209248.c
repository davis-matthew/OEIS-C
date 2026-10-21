/*
 * A209248  a(n) = 2^(2^n - 2*n - 1).
 * https://oeis.org/A209248
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2^n - 2*n - 1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp)           */
#include "../utils/big_pow.h"  /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* For n below the offset (n < 3) the value is undefined;
       returning 0 does not affect the sequence after offset. */
    if (n < 3) return (seq_t)0;

    /* Compute 2^n  */
    seq_t pow2n = pow2(n);                 /* 2^n */

    /* Compute 2*n  */
    seq_t twoN = ((seq_t)n) * 2;           /* 2*n */

    /* e = 2^n - 2*n - 1  */
    seq_t e = pow2n - twoN - (seq_t)1;     /* exponent for the final power */

    /* Final result: 2^e  */
    return big_pow((seq_t)2, e);
}
