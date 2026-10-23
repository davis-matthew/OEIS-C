/*
 * A043592  Numbers whose base-3 representation has exactly 12 runs.
 * https://oeis.org/A043592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generator for OEIS A043592: numbers whose base-3 representation has exactly 12 runs. */

#include <stdlib.h>

/* The function below computes the n-th number in sequence A043592.  It
 * lazily builds the sequence by enumerating all base‑3 strings with
 * exactly 12 runs, ordered first by length (shorter strings are
 * numerically smaller) and then lexicographically.  The results are
 * stored in a static dynamic array that grows as needed.  This
 * approach is O(1) per element after the initial prefilling of the
 * array. */
static seq_t *cache = NULL;          /* dynamic array of sequence values */
static size_t cache_size = 0;        /* number of values stored */
static size_t cache_cap   = 0;        /* current capacity of cache */
static unsigned lastL = 11;          /* highest length processed (init to 11) */

/* push a new value onto the dynamic array, reallocating if necessary */
static void cache_push(seq_t val) {
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap) ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* depth‑first recursion that builds a base‑3 string of length L
 * starting at position pos (next digit to place).  The current
 * number built is stored in val.  The current run count and the
 * previous digit are also tracked. */
static void gen_rec(int pos, int L, int runs, int prev, seq_t val) {
    if (pos == L) {
        if (runs == 12) cache_push(val);
        return;
    }
    int rem = L - pos - 1;          /* positions left after this one */

    for (int d = 0; d <= 2; ++d) {
        if (pos > 0 && d == prev) {            /* same run continues */
            if (runs > 12) continue;
            if (runs + rem < 12) continue;     /* cannot reach 12 runs */
            gen_rec(pos + 1, L, runs, prev, val * 3 + d);
        } else {                               /* new run starts */
            int new_runs = runs + 1;
            if (new_runs > 12) continue;
            if (new_runs + rem < 12) continue; /* cannot reach 12 runs */
            gen_rec(pos + 1, L, new_runs, d, val * 3 + d);
        }
    }
}

/* generate all base‑3 strings of exact length L whose run count is
 * 12, and append their decimal values to the cache.  The first
 * digit is chosen from {1,2} to avoid leading zeros. */
static void gen_len(int L) {
    for (int first = 1; first <= 2; ++first) {
        int pos = 1;                     /* next position to fill */
        int runs = 1;                    /* first run counted */
        seq_t val = first;
        if (L == 1) {                     /* should never happen for L>=12 */
            if (runs == 12) cache_push(val);
        } else {
            gen_rec(pos, L, runs, first, val);
        }
    }
}

/* Public API required by the build system.  Returns the n‑th term
 * of the sequence (1‑based index).  The cache is expanded lazily
 * until it contains at least n elements. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n=0, but harmless */
    while (cache_size < (size_t)n) {
        lastL++;                          /* next length to process */
        gen_len(lastL);
    }
    return cache[(size_t)n - 1];
}
