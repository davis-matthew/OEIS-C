/*
 * A043710  Numbers whose base-7 representation has an odd number of runs.
 * https://oeis.org/A043710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑7 representation *
 *            has an odd number of runs
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Return true if VAL has an odd number of runs in base‑7. */
static bool odd_runs(seq_t VAL)
{
    seq_t d = VAL;
    bool first = true;
    seq_t prev = 0;
    unsigned int trans = 0;

    while (d > 0) {
        seq_t digit = d % 7;
        if (first) {
            first = false;
        } else {
            if (digit != prev) trans++;
        }
        prev = digit;
        d /= 7;
    }
    /* Transitions even ↔ odd runs. */
    return (trans % 2) == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_candidate = 1;   /* number to test next */

    /* Return instantly if we already have the answer. */
    if (len >= n) return cache[n - 1];

    /* Initialise cache. */
    if (cap == 0) {
        cap = 128;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we reach the requested index. */
    while (len < n) {
        seq_t val = (seq_t)next_candidate;
        while (!odd_runs(val)) {
            ++val;
        }

        /* Grow cache if necessary. */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }

        cache[len++] = val;
        next_candidate = (idx_t)(val + 1);
    }

    return cache[n - 1];
}
