/*
 * A122986  Squares mod 1000.
 * https://oeis.org/A122986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, easy, nonn
 * Formula:   a(n) is the nth smallest residue of k² modulo 1000 (0 ≤ k <
 *            1000)
 * Range:     u8 n <= 43, u16 n <= 159, u32 n <= 159, u64 n <= 159,
 *            u128 n <= 159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache of all distinct quadratic residues modulo 1000,
       sorted increasingly.  The cache is built once and reused. */
    static seq_t *cache   = NULL;   /* array of residues */
    static size_t  count  = 0;      /* number of residues (159) */

    if (cache == NULL)               /* first call: build cache */
    {
        bool seen[1000] = {0};
        for (idx_t i = 0; i < 1000; ++i)
        {
            /* compute i² mod 1000 using 64‑bit arithmetic to avoid overflow
               even when seq_t is a small unsigned type. */
            uint64_t r = ((uint64_t)i * (uint64_t)i) % 1000ULL;
            seen[(int)r] = true;
        }

        /* Count the distinct residues. */
        for (int v = 0; v < 1000; ++v)
            if (seen[v]) ++count;

        cache = (seq_t *)malloc(sizeof(seq_t) * count);

        /* Fill the cache in sorted order (increasing residues). */
        size_t pos = 0;
        for (int v = 0; v < 1000; ++v)
            if (seen[v]) {
                cache[pos++] = (seq_t)v;
            }
    }

    /* The sequence is defined for 1 ≤ n ≤ count. */
    if (n == 0 || n > count)        /* out of bounds → 0 (undefined) */
        return (seq_t)0;

    return cache[n - 1];
}
