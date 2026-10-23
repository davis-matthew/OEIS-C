/*
 * A104268  a(n) = 2*4^(n-1) - (3n-1)/(2n+2)*C(2n,n).
 * https://oeis.org/A104268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^(n-1) - (3n-1)/(2n+2)*C(2n,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* n < 1 is outside the OEIS range */
    
    /* 4^(n-1) = 2^(2n-2) */
    idx_t exp = 2 * n - 2;
    seq_t part1 = pow2(exp);              /* 2^(2n-2) */
    part1 = part1 * ((seq_t)2);           /* 2 * 4^(n-1) */
    
    /* Binomial coefficient C(2n,n) */
    seq_t bin = binom(2 * n, n);
    
    /* (3n-1)*C(2n,n) / (2n+2) */
    seq_t num = bin * ((seq_t)(3 * n - 1));
    seq_t denom = (seq_t)(2 * n + 2);
    seq_t part2 = num / denom;
    
    return part1 - part2;
}
