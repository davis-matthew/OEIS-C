/*
 * A045082  Numbers whose base-4 representation contains exactly four 0's and
 *          one 3.
 * https://oeis.org/A045082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A045082: numbers whose base‑4 representation
   contains exactly four 0's and one 3, returned in increasing order. */

#include <stdlib.h>

/* persistent cache */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  checked_up_to = 0;

/* enlarge the cache when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) cache_cap = 64;
    else               cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* a(n) – n-th element of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined index, return 0 for safety */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        for (idx_t v = checked_up_to + 1; ; ++v) {
            idx_t   tmp    = v;
            unsigned zeros = 0, threes = 0;

            /* count base‑4 digits 0 and 3 */
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 4);
                if (d == 0) ++zeros;
                else if (d == 3) ++threes;
                tmp /= 4;
            }

            if (zeros == 4 && threes == 1) {
                if (cache_len == cache_cap) grow_cache();
                cache[cache_len++] = (seq_t)v;
            }

            if (cache_len >= n) {
                checked_up_to = v;   /* last number examined */
                break;
            }
        }
    }

    return cache[n - 1];   /* sequence is 1‑based */
}
