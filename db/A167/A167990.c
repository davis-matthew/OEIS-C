/*
 * A167990  Elements in A126988 (by row) that are not 1.
 * https://oeis.org/A167990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th value read row‑wise from A126988, skipping
 *            entries equal to 1 (none occur in this sequence)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cached = NULL;
static size_t cached_len  = 0;
static size_t cached_cap  = 0;
static idx_t  rows_done   = 0;   /* number of rows already generated */

/* Ensure the cache contains at least target entries. */
static void extend_cache_until(idx_t target)
{
    while (cached_len < target) {
        idx_t r = rows_done + 1;      /* next row number */

        for (idx_t k = 1; k <= r; ++k) {
            seq_t v;
            if (k == r) {
                v = 0;
            } else if (r % k == 0) {
                v = (seq_t)r / (seq_t)k;
            } else {
                v = 0;
            }
            if (v != (seq_t)1) {      /* skip value 1, but 0 is kept */
                if (cached_len == cached_cap) {
                    size_t new_cap = cached_cap ? cached_cap << 1 : 128;
                    seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
                    cached   = tmp;
                    cached_cap = new_cap;
                }
                cached[cached_len++] = v;
            }
        }
        rows_done++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range; offset starts at 1 */
    extend_cache_until(n);
    return cached[n - 1];
}
