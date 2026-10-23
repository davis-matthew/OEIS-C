/*
 * A043856  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 6 mod 8.
 * https://oeis.org/A043856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = kth positive integer whose base‑4 representation has a
 *            number of runs congruent to 6 modulo 8. (Runs are maximal blocks
 *            of equal consecutive digits)
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of already discovered sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next integer to test */
static seq_t next_candidate = (seq_t)1;

/* Count runs in the base‑4 representation of k.
   Runs are maximal strings of equal consecutive digits. */
static inline unsigned int runs_in_base4(seq_t k)
{
    unsigned int runs = 0;
    /* Sentinel value that cannot be a base‑4 digit (0‑3). */
    seq_t prev = (seq_t)4;
    while (k != (seq_t)0) {
        seq_t digit = k % (seq_t)4;
        k        /= (seq_t)4;
        if (digit != prev) {
            runs++;
            prev = digit;
        }
    }
    return runs;
}

/* Ensure the cache has space for one more element. */
static void ensure_capacity(void)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
#if defined(__cplusplus)
        /* Allocate a new array and copy existing elements. */
        seq_t *tmp = new (std::nothrow) seq_t[new_cap];
        if (!tmp) exit(1);
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        /* Use realloc for C builds. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
#endif
        cache_cap = new_cap;
    }
}

/* External linkage required by the judge. */
seq_t a(idx_t n)
{
    /* Off‑by‑one safety (a(0) is undefined in the OEIS offset). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Generate terms until the nth is available. */
    while (cache_len < (size_t)n) {
        unsigned int r = runs_in_base4(next_candidate);
        if (r % 8 == 6) {
            ensure_capacity();
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[(size_t)n - 1];
}
