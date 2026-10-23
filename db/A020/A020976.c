/*
 * A020976  Expansion of 1/((1-8*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A020976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3n+5) + 2^(n+1)*5^(n+2) - 9^(n+2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute the three powers */
    seq_t p1 = seq_pow((seq_t)2, (idx_t)(3 * n + 5));   /* 2^(3n+5)   */
    seq_t p2 = seq_pow((seq_t)2, (idx_t)(n + 1));       /* 2^(n+1)    */
    seq_t p3 = seq_pow((seq_t)5, (idx_t)(n + 2));       /* 5^(n+2)    */
    seq_t p4 = seq_pow((seq_t)9, (idx_t)(n + 2));       /* 9^(n+2)    */

    /* Multiply the middle terms: 2^(n+1) * 5^(n+2) */
    seq_t middle = seq_mul(p2, p3);

    /* Combine according to the closed form */
    return p1 + middle - p4;
}
