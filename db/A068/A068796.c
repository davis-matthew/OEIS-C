/*
 * A068796  Maximum k such that k consecutive integers starting at n have
 *          distinct numbers of prime factors (counted with multiplicity).
 * https://oeis.org/A068796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by brute‑force search: 
   find the maximum k such that the k consecutive integers
   n, n+1, … , n+k−1 have distinct numbers of prime factors
   (counted with multiplicity). */
#include <stdbool.h>
#include <string.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    enum { MAX_W = 256 };          /* size of table for seen omega values */
    static bool seen[MAX_W];        /* static so we can reuse memory */

    memset(seen, 0, sizeof(seen));

    idx_t k;
    for (k = 0; ; ++k) {
        seq_t om = bigomega(n + k);          /* omega for n+k */
        unsigned int w = (unsigned int)om;   /* omega fits into unsigned int */

        /* The unlikely case that w >= MAX_W is ignored – it never
           occurs for n up to at least 10⁶. */
        if (w >= MAX_W)
            continue;

        if (seen[w]) /* repeated omega detected */ 
            break;
        seen[w] = true;
    }

    return (seq_t)k;   /* number of distinct consecutive integers found */
}
