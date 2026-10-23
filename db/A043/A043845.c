/*
 * A043845  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 2 mod 7.
 * https://oeis.org/A043845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   base4_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043845: numbers k such that the number of runs in the
 * base‑4 representation of k is congruent to 2 modulo 7.
 * The function a(n) returns the n‑th such number (1‑based index). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>    /* for uintptr_t used only in abort */
#include "../utils/base4_runs.h"

static seq_t *cache      = NULL; /* cached terms */
static size_t cache_len  = 0;    /* number of valid terms stored */
static size_t cache_cap  = 0;    /* current allocation size */
static seq_t  last_k     = 0;    /* last integer tested */

/* Ensure that the cache can hold at least "need" more terms. */
static void ensure_capacity(size_t need)
{
    if (cache_cap - cache_len >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap - cache_len < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – terminate */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* index 0 is unused */
        return 0;

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    ensure_capacity(n - cache_len);

    while (cache_len < (size_t)n) {
        last_k = last_k + 1;      /* next integer to test */
        int r = base4_runs(last_k);
        if (r % 7 == 2)
            cache[cache_len++] = last_k;
    }

    return cache[n - 1];
}
