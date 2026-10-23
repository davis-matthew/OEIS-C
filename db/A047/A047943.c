/*
 * A047943  Number of 8-colorings of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A047943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^(n^2) + 2*8^((n^2 + 3(n mod 2))/4) + 8^((n^2 + (n mod
 *            2))/2))/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047943: Number of 8-colorings of an n X n grid, up to rotational symmetry */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arbitrary-precision arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t nmod2 = (seq_t)(n % 2);

    /* Exponents */
    seq_t e1 = nseq * nseq;                        /* n^2 */
    seq_t e2 = (e1 + nmod2 * (seq_t)3) / (seq_t)4; /* (n^2 + 3*(n mod 2))/4 */
    seq_t e3 = (e1 + nmod2) / (seq_t)2;           /* (n^2 + (n mod 2))/2 */

    /* Powers of 8 */
    const seq_t base = (seq_t)8;
    seq_t pow1 = big_pow(base, e1);
    seq_t pow2 = big_pow(base, e2);
    seq_t pow3 = big_pow(base, e3);

    /* Sum terms: pow1 + pow3 + 2*pow2, then divide by 4 */
    seq_t sum = pow1;
    sum += pow3;
    sum += pow2;
    sum += pow2;  /* add the second copy of pow2 */

    return sum / (seq_t)4;
}
