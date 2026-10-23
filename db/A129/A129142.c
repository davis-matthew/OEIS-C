/*
 * A129142  Start with the empty sequence and append in step k the consecutive
 *          numbers 2^k-1 to 2^k+k-2.
 * https://oeis.org/A129142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^k - 1 + (n - k(k-1)/2 - 1) where k is the smallest
 *            integer with k(k+1)/2 >= n
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   int_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find the block k that contains the nth term.
       We need smallest k with k(k+1)/2 >= n.
    */
    idx_t m = 1 + 8 * n;                     /* 1+8n for quadratic discriminant */
    idx_t s = int_sqrt(m);                   /* floor sqrt(1+8n) */
    idx_t k = (s - 1) / 2;                   /* floor of (-1+sqrt)/2 */
    idx_t cum = k * (k + 1) / 2;             /* cumulative count up to block k */
    if (cum < n) {
        ++k;                                 /* adjust to next block if needed */
        cum = k * (k + 1) / 2;
    }
    /* Position within block k (0‑based) */
    idx_t prev_cum = k * (k - 1) / 2;
    idx_t offset = n - prev_cum - 1;
    /* Compute the term: start of block k is 2^k - 1, then add offset */
    seq_t term = pow_seq((seq_t)2, (int)k);   /* 2^k as seq_t */
    term = term + (seq_t)offset - 1;          /* 2^k - 1 + offset */
    return term;
}
