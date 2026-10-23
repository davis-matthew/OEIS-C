/*
 * A106400  Thue-Morse sequence: let A_k denote the first 2^k terms; then A_0
 *          = 1 and for k >= 0, A_{k+1} = A_k B_k, where B_k is obtained from
 *          A_k by interchanging 1's and -1's.
 * https://oeis.org/A106400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^wt(n), where wt(n) is the binary Hamming weight of n
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute the parity of the binary weight of n.
       Even weight => 1, odd weight => -1. */
    unsigned long long x = (unsigned long long)n;
    /* Count bits set to 1.  Use builtin where available. */
    unsigned int pop = __builtin_popcountll(x);
    return (pop & 1) ? (seq_t)-1 : (seq_t)1;
}
