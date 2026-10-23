/*
 * A014311  Numbers with exactly 3 ones in binary expansion.
 * https://oeis.org/A014311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 560, u32 n <= 4960, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014311: numbers with exactly 3 ones in their binary expansion.
   The nth term is obtained by enumerating all triples (a,b,c) with
   0≤c<b<a, sorted lexicographically (a then b then c).  For each triple
   the value is 2^a + 2^b + 2^c. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the smallest a≥2 such that C(a+1,3) ≥ n.
       C(a+1,3) is the total number of triples with highest bit ≤ a.
       C(a,3) is the number of triples with highest bit ≤ a-1. */
    for (idx_t a = 2; ; ++a)
    {
        /* Compute C(a+1,3) */
        idx_t count = (a + 1) * a * (a - 1) / 6;

        if (count >= n)
        {
            /* Offset of n within the block of triples having highest bit a. */
            idx_t prev = 0;
            if (a >= 3)
                prev = a * (a - 1) * (a - 2) / 6;

            idx_t offset = n - prev;            /* 1‑based within block   */
            idx_t q      = offset - 1;           /* 0‑based index         */

            /* Find b such that cumulative pairs exceed q.
               Number of pairs for a given b is b (c ranges 0..b-1). */
            idx_t b;
            for (b = 1; ; ++b)
                if (b * (b + 1) / 2 > q)
                    break;

            idx_t cumulative_prev = (b - 1) * b / 2;
            idx_t c = q - cumulative_prev;      /* 0‑based c within block */

            return pow2(a) + pow2(b) + pow2(c);
        }
    }
}
