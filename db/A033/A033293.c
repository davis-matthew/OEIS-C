/*
 * A033293  A Connell-like sequence: take 1 number = 1 (mod Q), 2 numbers = 2
 *          (mod Q), 3 numbers = 3 (mod Q), etc., where Q = 8.
 * https://oeis.org/A033293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033293 – Connell‑like sequence
 *
 * The sequence is built as follows: for m = 1, 2, 3, … take
 * m numbers which are strictly greater than the previous number
 * and congruent to m (mod 8). The terms are taken in increasing order.
 *
 * Example: 1 (m=1,r=1), 2,10 (m=2,r=2), 11,19,27 (m=3,r=3),
 * 28,36,44,52 (m=4,r=4), …
 *
 * This function returns the n‑th term (a(1)=1).  It caches already
 * computed terms so that any call order is supported.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* state of the generation algorithm */
    static idx_t block   = 1;   /* current block index (m) */
    static idx_t taken   = 0;   /* numbers taken in current block */
    static uint64_t cur64 = 0;  /* real current value (may exceed seq_t) */

    if (n == 0) return 0;  /* undefined for n==0, harmless to return 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* residue for the current block (0..7) */
        int r = block % 8;

        /* difference needed to reach the next number ≡ r (mod 8) */
        int rem  = (int)(cur64 % 8);
        int diff = (r - rem + 8) % 8;
        if (diff == 0)
            diff = 8;               /* must be strictly greater than cur64 */

        /* new term */
        uint64_t next64 = cur64 + (uint64_t)diff;

        /* grow cache if necessary */
        if (cache_len + 1 > cache_cap)
        {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!newp) abort();
            cache = newp;
        }

        /* store term */
        cache[cache_len++] = (seq_t)next64;
        cur64 = next64;

        /* update block state */
        taken++;
        if (taken == block)
        {
            block++;
            taken = 0;
        }
    }

    return cache[n - 1];   /* zero‑based indexing in the cache */
}
