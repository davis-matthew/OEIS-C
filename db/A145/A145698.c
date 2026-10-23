/*
 * A145698  Numbers x such that (x+37)^3 - x^3 is a square.
 * https://oeis.org/A145698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145698: numbers x such that (x+37)^3 - x^3 is a square.
   Recurrence (derived from the OEIS b-file):
        a(1) = 111
        a(2) = 76516
        a(n+2) = 590*a(n+1) - a(n) + 10878    for n ≥ 1
   The recurrence is used for all values; no lookup table is employed. */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence storage */
static seq_t *seq_cache = NULL;      /* seq_cache[1] holds a(1) */
static idx_t  seq_cached = 0;        /* highest index that is cached (≤ seq_cached) */
static size_t seq_capacity = 0;      /* allocated size of seq_cache */

/* Ensure capacity for indices up to at least n (n ≥ 1). */
static void grow_cache(idx_t n)
{
    if (seq_capacity >= (size_t)(n + 1))
        return;

    size_t new_cap = seq_capacity == 0 ? 16 : seq_capacity * 2;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Allocate a new buffer and copy old contents */
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                        /* out‑of‑memory */
    seq_cache = tmp;
    seq_capacity = new_cap;
}

/* Extend the cached sequence up to index n using the recurrence. */
static void extend_cache(idx_t n)
{
    if (seq_cached >= n)
        return;

    grow_cache(n);

    /* Seed initial values if they are not yet present */
    if (seq_cached == 0) {
        seq_cache[1] = (seq_t)111;      /* a(1) */
        seq_cached = 1;
    }
    if (seq_cached == 1 && n >= 2) {
        seq_cache[2] = (seq_t)76516;    /* a(2) */
        seq_cached = 2;
    }

    /* Compute subsequent values by recurrence */
    for (idx_t i = seq_cached + 1; i <= n; ++i) {
        /* a(i) = 590*a(i-1) - a(i-2) + 10878 */
        seq_t val = (seq_t)590 * seq_cache[i - 1] - seq_cache[i - 2] + (seq_t)10878;
        seq_cache[i] = val;
    }
    seq_cached = n;
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* undefined for n < 1 */

    extend_cache(n);
    return seq_cache[n];
}
