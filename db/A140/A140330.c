/*
 * A140330  Smallest nonprime with Hamming weight n (i.e., with exactly n 1's
 *          when written in binary).
 * https://oeis.org/A140330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = MIN{ k | k has Hamming weight n and k is nonprime }
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 16, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over numbers with n set bits starting from the
   Mersenne number 2^n – 1, testing each for primality with Gosper's hack
   to generate the next integer with the same number of 1 bits. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* For n == 0 the sequence is undefined; return 0 safely. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Start from the smallest n‑bit number consisting entirely of 1's. */
    seq_t cand = seq_pow((seq_t)2, n) - (seq_t)1;

    /* Find the first non‑prime among numbers with popcount n. */
    while (is_prime_big(cand)) {
        /* Gosper's hack: next integer with the same popcount. */
        seq_t smallest = cand & -cand;          /* lowest set bit      */
        seq_t ripple   = cand + smallest;       /* add lowest set bit  */
        seq_t ones     = cand ^ ripple;         /* bits that flipped   */
        ones = (ones >> 2) / smallest;          /* rearrange higher bits */
        cand = ripple | ones;                   /* new candidate       */
    }

    return cand;
}
