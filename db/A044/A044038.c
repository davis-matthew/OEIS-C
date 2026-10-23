/*
 * A044038  Numbers k such that 7 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044038: numbers k such that 7 and 8 occur juxtaposed in
   base‑10 representation of k but not of k+1.
   Computed by a simple linear search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

static inline bool has_adjacent(seq_t x)
{
    if (x < 10) return false;          /* need at least two digits */
    seq_t prev = x % 10;               /* the least significant digit */
    x /= 10;
    while (x > 0) {
        seq_t cur = x % 10;
        if ((cur == 7 && prev == 8) || (cur == 8 && prev == 7))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* static memoisation storage */
static seq_t *cache      = NULL;   /* array of found terms               */
static size_t cache_len  = 0;      /* number of terms currently stored   */
static size_t cache_cap  = 0;      /* capacity of the array               */
static seq_t last_scanned = 0;     /* last integer examined              */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below offset is irrelevant */

    /* lazy initialisation of the cache */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate more terms until we reach index n */
    while (cache_len < n) {
        seq_t k = last_scanned + 1;
        last_scanned = k;

        if (has_adjacent(k)) {
            seq_t kp1 = k + 1;
            if (!has_adjacent(kp1)) {
                /* grow the cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
    }

    return cache[n - 1];
}
