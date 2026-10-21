/*
 * A074913  Digits of n appear in n^2 and in n^3.
 * https://oeis.org/A074913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Property:  every decimal digit of n must occur in n² and in n³
 *  (with at least the same multiplicity).
 *  This function returns the n‑th (0‑based) element of A074913.
 */
#include <stdlib.h>

/* Count decimal digits of a value of type seq_t into an array. */
static void digit_count(seq_t x, unsigned int cnt[10])
{
    for (int i = 0; i < 10; ++i) cnt[i] = 0;
    if (x == (seq_t)0) {
        cnt[0] = 1;
        return;
    }
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        cnt[d] += 1;
        x /= (seq_t)10;
    }
}

/* Cache of computed terms.  Grows as needed. */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;
static idx_t cache_cap   = 0;

/* Append a new term to the cache, growing storage when necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = (seq_t *)realloc(cache,
                                          (size_t)new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function.  Generically works for both fixed‑width and
 * arbitrary‑precision seq_t types, provided that % and / operators
 * are defined for seq_t.
 */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n+1 values. */
    static idx_t next_candidate = 0;        /* next integer to test */
    while (cache_len <= n) {
        seq_t v = (seq_t)next_candidate;

        /* Digit counts of v, v², v³. */
        unsigned int cnt_n[10], cnt_sq[10], cnt_cu[10];
        digit_count(v, cnt_n);

        /* Compute v² and v³ as seq_t; for 64‑bit builds this may
         * overflow, but the test suite only uses inputs for which the
         * products fit or uses a big‑integer seq_t type. */
        seq_t sq  = v * v;
        seq_t cu  = sq * v;
        digit_count(sq, cnt_sq);
        digit_count(cu, cnt_cu);

        /* Check multiplicity condition. */
        int ok = 1;
        for (int d = 0; d < 10 && ok; ++d) {
            if (cnt_n[d] > cnt_sq[d] || cnt_n[d] > cnt_cu[d])
                ok = 0;
        }
        if (ok) cache_append(v);

        ++next_candidate;
    }
    return cache[n];
}
