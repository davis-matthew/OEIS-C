/*
 * A183640  Number of (n+1) X 8 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 52670 + 3025*2^(2+n) + 254*3^(1+n) + 4^(1+n) (offset 1)
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp)   */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b)       */

seq_t a(idx_t n)
{
    /* For safety with the offset: return 0 for n=0, though a(1) is the first term. */
    if (n == 0)
        return 0;

    seq_t result = (seq_t)52670;

    /* 2^(2+n) and multiply by 3025 */
    seq_t pow2 = seq_pow((seq_t)2, n + 2);          /* 2^(2+n) */
    seq_t term2 = seq_mul((seq_t)3025, pow2);      /* 3025 * 2^(2+n) */

    /* 3^(1+n) and multiply by 254 */
    seq_t pow3 = seq_pow((seq_t)3, n + 1);          /* 3^(1+n) */
    seq_t term3 = seq_mul((seq_t)254, pow3);       /* 254 * 3^(1+n) */

    /* 4^(1+n) */
    seq_t pow4 = seq_pow((seq_t)4, n + 1);          /* 4^(1+n) */

    /* Sum all contributions */
    result = result + term2 + term3 + pow4;

    return result;
}
