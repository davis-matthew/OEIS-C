/*
 * A140467  Numbers k such that the digits of k are not present in 3k.
 * https://oeis.org/A140467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 2681, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal digits of k are disjoint from those of 3k.
   Computed by brute‑force search with memoization. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cached sequence terms */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of terms stored */
static seq_t next_k = 1;          /* next integer to test */

/* Return true if k and 3*k share no common decimal digit. */
static bool digits_disjoint(seq_t k)
{
    seq_t t = k * 3;
    unsigned int fk[10] = {0};
    unsigned int fm[10] = {0};

    digit_frequencies(k, fk);
    digit_frequencies(t, fm);

    for (int d = 0; d < 10; ++d)
        if (fk[d] && fm[d])
            return false;
    return true;
}

/* Grow the cache array when it becomes full. */
static void ensure_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp)
            exit(EXIT_FAILURE);             /* simple error handling */
        cache     = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1 – a(1) is the first term.  For safety, return 0 for n==0. */
    if (n == 0) return 0;

    /* Generate terms until we have the nth term. */
    while (cache_len < (size_t)n) {
        bool ok = digits_disjoint(next_k);
        if (ok) {
            ensure_capacity();
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
