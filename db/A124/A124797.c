/*
 * A124797  Sum of cyclic permutations of 123...n seen as number written in
 *          base n+1: ((n+1)^n-1)*(n+1)/2.
 * https://oeis.org/A124797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)^n - 1) * (n+1) / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Base (n+1) as seq_t */
    seq_t base = (seq_t)(n + 1ULL);

    /* Compute (n+1)^n */
    seq_t pow = big_pow(base, (seq_t)n);

    /* Apply the formula */
    seq_t result = (pow - (seq_t)1) * base / (seq_t)2;
    return result;
}
