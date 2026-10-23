/*
 * A024518  a(n) = position of 1 + n^2 in A000404 (sums of 2 nonzero squares).
 * https://oeis.org/A024518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024518:
   a(n) = position of 1 + n^2 among numbers that are a sum
          of two non‑zero squares (A000404) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build supplies the following types:
     idx_t – unsigned 64‑bit index
     seq_t – unsigned ≥8‑bit (big integer on some builds)   */
seq_t a(idx_t n)
{
    /* Static state for lazy, incremental computation */
    static idx_t          max_n      = 0;   /* largest n already computed   */
    static idx_t          max_limit  = 0;   /* 1 + max_n^2                 */
    static seq_t          *results    = NULL;/* results[1..max_n]           */
    static unsigned char  *bits       = NULL;/* bit vector for sums          */

    /* Recompute up to new_max_n if necessary */
    if (n > max_n)
    {
        idx_t new_max_n  = n;
        idx_t new_limit  = 1 + new_max_n * new_max_n;

        /* Allocate bit vector (ceil(new_limit+1)/8 bytes) */
        size_t bits_size = (new_limit >> 3) + 1;
        unsigned char *new_bits = (unsigned char *)calloc(bits_size, 1);
        if (!new_bits) return 0; /* allocation failed */

        /* Mark all sums x^2 + y^2 with x,y ≥ 1 */
        for (idx_t x = 1; x * x <= new_limit; ++x)
        {
            seq_t x2 = (seq_t)x * (seq_t)x;
            for (idx_t y = 1; ; ++y)
            {
                seq_t sum = x2 + (seq_t)y * (seq_t)y;
                if (sum > (seq_t)new_limit) break;
                idx_t s = (idx_t)sum;
                new_bits[s >> 3] |= (unsigned char)(1u << (s & 7));
            }
        }

        /* Allocate result array (indices 1..new_max_n) */
        seq_t *new_results = (seq_t *)calloc(new_max_n + 1, sizeof(seq_t));
        if (!new_results)
        {
            free(new_bits);
            return 0;
        }

        /* Scan numbers in order, counting representable ones.
           Store the count when reaching each target = 1 + k^2. */
        seq_t cur = 0;
        idx_t next_k = 1;
        idx_t target = 1 + 1; /* 1 + 1^2 */
        for (idx_t i = 1; i <= new_limit; ++i)
        {
            if (new_bits[i >> 3] & (1u << (i & 7)))
                ++cur;
            if (i == target)
            {
                new_results[next_k] = cur;
                ++next_k;
                if (next_k > new_max_n) break;
                target = 1 + next_k * next_k;
            }
        }

        /* Replace old storage with the new blocks */
        free(bits);
        free(results);
        bits      = new_bits;
        results   = new_results;
        max_n     = new_max_n;
        max_limit = new_limit;
    }

    /* Return the stored result (0 if n is out of bounds or n == 0). */
    if (n == 0 || n > max_n) return 0;
    return results[n];
}
