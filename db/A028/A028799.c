/*
 * A028799  Nonsquares mod 86.
 * https://oeis.org/A028799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th positive integer that is NOT a quadratic residue
 *            modulo 86
 * Range:     u8 n <= 42, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool residue_found[86];
static bool residue_initialized = false;

static void init_residues(void)
{
    if (residue_initialized) return;
    for (int i = 0; i < 86; ++i) residue_found[i] = false;
    for (uint64_t i = 0; i < 86; ++i) {
        uint64_t sq = (i * i) % 86;
        residue_found[(unsigned int)sq] = true;
    }
    residue_initialized = true;
}

/* Dynamic cache of computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of cached terms */
static uint64_t last_value = 1; /* next integer to test, start at 1 */

static void ensure_cache_size(size_t needed)
{
    if (cache_len >= needed) return;
    /* Grow cache if necessary */
    if (cache_size == 0) {
        cache_size = 64;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_len < needed) {
        /* Find next nonsquare modulo 86 */
        while (true) {
            unsigned int r = (unsigned int)(last_value % 86u);
            if (!residue_found[r]) break;
            ++last_value;
        }
        /* Store it */
        if (cache_len == cache_size) {
            size_t new_size = cache_size * 2;
            seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!new_cache) abort();
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
            cache = new_cache;
            cache_size = new_size;
        }
        cache[cache_len] = (seq_t)last_value;
        ++cache_len;
        ++last_value;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based */
    if (n == 0) return 0;
    init_residues();
    ensure_cache_size((size_t)n);
    return cache[(size_t)n - 1];
}
