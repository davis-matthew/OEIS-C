/*
 * A100922  k appears A000120(k) times (appearances equal number of 1-bits).
 * https://oeis.org/A100922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 9393, u32 n <= 9393, u64 n <= 9393,
 *            u128 n <= 9393, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the least k such that the cumulative sum of the
 * number of 1‑bits in 1 … k exceeds n.  Equivalently, each integer
 * k appears popcount(k) times in the sequence.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cache for previously computed terms. */
static seq_t *cache = NULL;   /* dynamically allocated array */
static size_t cap   = 0;      /* allocated capacity */
static size_t sz    = 0;      /* number of filled entries */

/* Current integer whose repetitions are being output. */
static uint64_t cur_k     = 1;   /* the integer k to output */
static size_t  cur_remain = 1;   /* how many times k should still be output */

/* Popcount for a 64 bit integer. */
static inline size_t popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)   /* builtin available */
    return __builtin_popcountll(x);
#else
    /* Kernighan's algorithm */
    size_t c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

/* Expand the cache so that index n is reachable. */
static void extend_cache(idx_t n)
{
    /* Allocate the initial storage if needed. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms up to index n. */
    while (sz <= (size_t)n) {
        /* Grow the buffer when necessary. */
        if (sz >= cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!cache) abort(); /* unrecoverable memory error */
        }

        /* Store the current value. */
        cache[sz++] = (seq_t)cur_k;

        /* Update remainder / advance to next k when necessary. */
        if (cur_remain <= 1) {
            ++cur_k;
            cur_remain = popcount64(cur_k);
        } else {
            --cur_remain;
        }
    }
}

/* Public function: returns a(n) for any n ≥ 0. */
extern seq_t a(idx_t n)
{
    if ((size_t)n >= sz) {
        extend_cache(n);
    }
    return cache[n];
}
