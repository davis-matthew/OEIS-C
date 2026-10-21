/*
 * A090822  Gijswijt's sequence: a(1) = 1; for n>1, a(n) = largest integer k
 *          such that the word a(1)a(2)...a(n-1) is of the form xy^k for words
 *          x and y (where y has positive length), i.e., the maximal number of
 *          repeating blocks at the end of the sequence so far.
 * https://oeis.org/A090822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 9099, u16 n <= 9099, u32 n <= 9099, u64 n <= 9099,
 *            u128 n <= 9099, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the largest integer k such that the word a(1)…a(n‑1) can be written as x y^k
   for some words x, y with |y| > 0. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;   /* 0‑based cache: seq_cache[0] = a(1) */
static idx_t seq_len  = 0;        /* number of terms already stored */
static idx_t cache_cap = 0;       /* allocated capacity of seq_cache */

/* Ensure the cache can hold at least 'needed' terms (needed ≥ seq_len). */
static void ensure_cache(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Generate the next term after the current seq_len terms. */
static void generate_next(void)
{
    idx_t len = seq_len;              /* number of terms known so far */
    ensure_cache(len + 1);            /* make room for a(len+1) */

    seq_t best = 0;                   /* best k found for this step */

    /* Try every possible length of y (the repeating block). */
    for (idx_t block_len = 1; block_len <= len; ++block_len) {
        idx_t start = len - block_len;   /* start index of the last block y */
        idx_t count = 1;                 /* at least one occurrence of y */

        /* Count how many consecutive copies of y appear before the last one. */
        while (start >= block_len) {
            bool match = true;
            for (idx_t j = 0; j < block_len; ++j) {
                if (seq_cache[start + j] != seq_cache[start - block_len + j]) {
                    match = false;
                    break;
                }
            }
            if (!match) break;
            ++count;
            start -= block_len;
        }
        if (count > best) best = count;
    }

    seq_cache[seq_len] = best;
    ++seq_len;
}

/* External function required by the task. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but harmless */

    /* Ensure the first two terms are present. */
    if (seq_len < 1) {
        ensure_cache(2);
        seq_cache[0] = (seq_t)1;      /* a(1) */
        seq_cache[1] = (seq_t)1;      /* a(2) */
        seq_len = 2;
    }

    /* Grow the cache until it contains a(n). */
    while (seq_len < n)
        generate_next();

    return seq_cache[n - 1];
}
