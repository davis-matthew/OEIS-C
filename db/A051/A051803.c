/*
 * A051803  Numbers with nonzero multiplicative digital root 1.
 * https://oeis.org/A051803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Numbers with nonzero multiplicative digital root 1 (OEIS
 *            A051803)
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   mdr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mdr.h"

/* --- cached generation of the sequence --- */
static idx_t  next_check = 0;   /* next natural number to test */
static idx_t  cache_len  = 0;   /* how many terms have been cached */
static size_t cache_cap  = 0;   /* allocated capacity of cache */
static seq_t *cache      = NULL;

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined, but safe fall‑back */

    while (cache_len < n) {
        seq_t val = next_check;
        bool good = (val == (seq_t)0) || (mdr(val) == (seq_t)1);
        if (good) {
            if (cache_cap == 0) {
                cache_cap = 8;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else if (cache_len + 1 > cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
        ++next_check;
    }
    return cache[n - 1];
}
