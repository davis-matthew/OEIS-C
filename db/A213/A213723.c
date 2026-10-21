/*
 * A213723  a(n) = smallest natural number x such that x=n+A000120(x),
 *          otherwise zero.
 * https://oeis.org/A213723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by
   a(n) = smallest natural number x such that x = n + A000120(x),
   where A000120(x) is the number of 1‑bits (popcount) in the binary
   expansion of x.  If no such x exists, a(n) = 0.
   The implementation uses the formula x = n + popcount(x); hence
   for every candidate k = popcount(x) we check whether x = n + k
   satisfies popcount(x) = k.  This yields the smallest solution
   (or none).  The algorithm is O(k) per call, where k is the
   smallest popcount satisfying the condition (at most a few dozen
   for n ≤ 10000).  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Helper: compute popcount for a seq_t value */
static inline seq_t popcount_seq(seq_t v)
{
    seq_t c = 0;
    seq_t w = v;
    while (w) {
        c += w & 1;
        w >>= 1;
    }
    return c;
}

/* Entry point for the sequence */
seq_t a(idx_t n)
{
    /* n = 0 has the trivial solution x = 0 */
    if (n == 0)
        return 0;

    seq_t ns = (seq_t)n;
    seq_t k;

    /* k = 0 would give x = n which fails for n > 0 because
       popcount(n) ≥ 1.  Start from k = 1. */
    for (k = 1; ; ++k) {
        seq_t x = ns + k;

        seq_t pc = popcount_seq(x);
        if (pc == k)
            return x;

        /* No solution can exist once k exceeds the bit‑length of x.
           bitlen_seq_t returns the required number of bits. */
        if (k > (seq_t)bitlen_seq_t(x))
            break;
    }

    return 0;   /* no solution for this n */
}
