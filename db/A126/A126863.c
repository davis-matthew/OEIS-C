/*
 * A126863  S(1)={1}. S(n) = {S(n-1) {1,2,...,(n-1),n,(n-1),...,2,1} S(n-1)},
 *          where S(n) is a string of the first 3*2^n -2*n -3 terms of the
 *          sequence.
 * https://oeis.org/A126863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6119, u16 n <= 6119, u32 n <= 6119, u64 n <= 6119,
 *            u128 n <= 6119, big any n
 * Helpers:   seq_a_recurse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126863 – a(n) is the n-th term of the infinite palindrome
   string constructed by S(1)= {1} and
   S(k) = S(k-1) ∘ [1,2,…,k,…,2,1] ∘ S(k-1).
   The formula used below follows from this recursive definition. */

#include <stdint.h>
#include "../utils/seq_a_recurse.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of defined range – not used */
    return seq_a_recurse(n);
}
