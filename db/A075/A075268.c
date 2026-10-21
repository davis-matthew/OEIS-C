/*
 * A075268  Trajectory of 442 under the Reverse and Add! operation carried out
 *          in base 2.
 * https://oeis.org/A075268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 31, u64 n <= 95, u128 n <= 223,
 *            big any n
 * Helpers:   reverse_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075268:  Trajectory of 442 under the Reverse-and‑Add operation
 * in base 2.
 *
 * Recurrence (integer indices, n >= 0):
 *   a(0) = 442
 *   a(n+1) = a(n) + reverse_bits(a(n))
 * where reverse_bits(x) is the binary bit‑reversal of x using its
 * leading‑bit length (i.e., no leading zeros are added or removed).
 *
 * The implementation computes values on demand and stores all previously
 * computed terms in a dynamic array so that any order of queries is
 * handled efficiently.  It works for all supported seq_t types,
 * including arbitrary‑precision big integer classes used in the build.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_bits.h"

/* ---- storage for computed sequence values --------------------------------*/
static seq_t *cache   = NULL;  /* array of a(k) values */
static size_t cache_size = 0;  /* number of valid entries in cache */
static size_t cache_cap  = 0;  /* allocated capacity */

/* ---- main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)442;
        } else {
            seq_t prev = cache[i - 1];
            seq_t rev  = reverse_bits(prev);
            cache[i] = prev + rev;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
