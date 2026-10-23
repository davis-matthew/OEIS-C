/*
 * A196221  a(n) = binomial(n+10, 10)*9^n.
 * https://oeis.org/A196221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10)*9^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */
#include "../utils/seq_pow.h" /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* b = C(n+10,10) */
    seq_t b = binom(n + 10, 10);

    /* p = 9^n */
    seq_t p = seq_pow((seq_t)9, n);

    /* a(n) = b * p; multiplication works for both builtin
       integer types and arbitrary‑precision classes. */
    return b * p;
}
