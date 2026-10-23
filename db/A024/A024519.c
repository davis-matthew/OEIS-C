/*
 * A024519  Position of n^2 + (n+1)^2 in A000404 (sums of 2 nonzero squares).
 * https://oeis.org/A024519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n^2 + (n+1)^2 in A000404 (sums of two
 *            non‑zero squares)
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Function declaration – seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n);

/* ---------- Integer square root for idx_t ---------- */
static inline idx_t isqrt_idx(idx_t v)
{
    /* Use double sqrt as an efficient starting point; for values up
       to 2^63 the error is less than 1, so the following fixes are
       trivial. */
    idx_t r = (idx_t) sqrt((double) v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

/* ---------- Cache of representable numbers up to current maximum ---------- */
static uint64_t *bitset = NULL;   /* one bit per integer */
static uint32_t *prefix = NULL;   /* cumulative popcount per 64‑bit word */
static idx_t max_limit = 0;       /* maximum integer currently covered */
static idx_t used_words = 0;      /* number of 64‑bit words actually allocated */

/* Ensure all numbers ≤ limit that can be expressed as a^2 + b^2 with
   a, b ≥ 1 are marked in the bitset. Prefix counts are updated. */
static void ensure_cache(idx_t limit)
{
    if (limit <= max_limit) return;            /* already up‑to‑date */

    /* Resize bitset */
    idx_t new_words = (limit >> 6) + 1;
    if (bitset == NULL) {
        bitset = (uint64_t *)calloc(new_words, sizeof(uint64_t));
    } else {
        bitset = (uint64_t *)realloc(bitset, new_words * sizeof(uint64_t));
        /* Zero any newly allocated words */
        for (idx_t i = used_words; i < new_words; ++i) bitset[i] = 0;
    }

    /* Mark all sums a^2 + b^2 (a, b ≥ 1) up to limit, using a ≤ b to
       avoid duplicates. */
    idx_t a_max = isqrt_idx(limit);
    for (idx_t a = 1; a <= a_max; ++a) {
        idx_t a2 = a * a;
        idx_t b_max = isqrt_idx(limit - a2);
        if (b_max < a) continue;             /* no valid b */
        for (idx_t b = a; b <= b_max; ++b) {
            idx_t sum = a2 + b * b;
            bitset[sum >> 6] |= 1ULL << (sum & 63);
        }
    }

    /* Recompute prefix popcounts */
    uint32_t *new_prefix = (uint32_t *)malloc((new_words + 1) * sizeof(uint32_t));
    new_prefix[0] = 0;
    for (idx_t i = 0; i < new_words; ++i) {
        new_prefix[i + 1] = new_prefix[i] + __builtin_popcountll(bitset[i]);
    }
    free(prefix);
    prefix = new_prefix;

    used_words = new_words;
    max_limit  = limit;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* Compute m = n^2 + (n+1)^2 as seq_t to avoid overflow of idx_t. */
    seq_t nn = (seq_t)n;
    seq_t m  = nn * nn + (nn + 1) * (nn + 1);

    /* Ensure the cache covers m.  The cast to idx_t is safe for the
       intended range of n (up to about 10 000). */
    ensure_cache((idx_t)m);

    /* Count representable numbers ≤ m. */
    idx_t word  = (idx_t)m >> 6;
    idx_t off   = (idx_t)m & 63;
    uint64_t mask = (off == 63) ? ~0ULL : ((1ULL << (off + 1)) - 1);

    uint32_t count = prefix[word] + __builtin_popcountll(bitset[word] & mask);

    return (seq_t)count;
}
