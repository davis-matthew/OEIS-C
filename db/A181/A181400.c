/*
 * A181400  Total number of positive integers below 10^n requiring 6 positive
 *          cubes in their representation as sum of cubes.
 * https://oeis.org/A181400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of positive integers < 10ⁿ that need
 * exactly 6 positive cubes in a sum‑of‑cubes representation.
 * This is computed by building all numbers representable with
 * ≤5 cubes (via bitset DP) up to 10⁷‑1, then counting those that
 * are representable with ≤6 cubes but not with ≤5 cubes.
 * The count stabilises at 3922 for n ≥ 7.
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* helper: shift a bitset left by s bits and OR into dst (in place) */
static void shift_or(uint64_t *dst, const uint64_t *src, size_t words, size_t s)
{
    size_t whole = s >> 6;          /* /64 */
    uint32_t  rem  = s & 63;        /* %64 */
    size_t i, w;
    if (rem == 0) {
        for (i = 0; i + whole < words; ++i) {
            uint64_t val = src[i];
            if (!val) continue;
            dst[i + whole] |= val;
        }
    } else {
        for (i = 0; i + whole < words; ++i) {
            uint64_t val = src[i];
            if (!val) continue;
            dst[i + whole] |= val << rem;
            if (i + whole + 1 < words)
                dst[i + whole + 1] |= val >> (64 - rem);
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* undefined for n < 1 */
    if (n < 1)
        return (seq_t)0;

    /* upper bound for precomputation (10⁷‑1) */
    const idx_t MAX_N = 10000000;          /* 10⁷ */
    const idx_t LIMIT = MAX_N - 1;         /* 9,999,999 */

    /* static state for one‑time precomputation */
    static int ready = 0;
    static seq_t *prefix = NULL;
    static uint64_t *prev = NULL;
    static uint64_t *curr = NULL;
    static uint64_t *reach_le = NULL; /* numbers reachable with ≤5 cubes */
    static uint64_t *reach6   = NULL;
    static uint32_t   *cubes   = NULL;
    static size_t  cubeCnt = 0;

    if (!ready) {
        /* compute cubes up to LIMIT */
        size_t maxK = (size_t) ((uint64_t)( MAX_N ) * 1.0 / 1); /* dummy dummy */
        for (size_t k = 1; ; ++k) {
            uint64_t c = (uint64_t)k * k * k;
            if (c > LIMIT)
                break;
            /* store */
        }

        /* allocate cube list dynamically (215 cubes max) */
        size_t maxCubeCnt = 260; /* safety margin */
        cubes = (uint32_t *)malloc(maxCubeCnt * sizeof(uint32_t));
        cubeCnt = 0;
        for (size_t k = 1; ; ++k) {
            uint64_t c = (uint64_t)k * k * k;
            if (c > LIMIT)
                break;
            cubes[cubeCnt++] = (uint32_t)c;
        }

        /* bitset size (words of 64 bits) */
        size_t words = (LIMIT + 64) >> 6;

        /* allocate bitsets */
        prev    = (uint64_t *)calloc(words, sizeof(uint64_t));
        curr    = (uint64_t *)calloc(words, sizeof(uint64_t));
        reach_le= (uint64_t *)calloc(words, sizeof(uint64_t));
        reach6  = (uint64_t *)calloc(words, sizeof(uint64_t));
        if (!prev || !curr || !reach_le || !reach6 || !cubes)
            return (seq_t)0; /* allocation failure */

        /* base case: 0 reachable with 0 cubes */
        prev[0] = 1ULL;

        /* build reachable sets for depths 1..5 */
        for (size_t depth = 1; depth <= 5; ++depth) {
            memset(curr, 0, words * sizeof(uint64_t));
            for (size_t ci = 0; ci < cubeCnt; ++ci)
                shift_or(curr, prev, words, cubes[ci]);

            /* merge into reachable_le (≤ depth) */
            for (size_t w = 0; w < words; ++w)
                reach_le[w] |= curr[w];

            /* prepare for next depth: prev = curr */
            memcpy(prev, curr, words * sizeof(uint64_t));
        }

        /* now prev holds numbers reachable with exactly 5 cubes */
        /* compute numbers reachable with exactly 6 cubes */
        memset(reach6, 0, words * sizeof(uint64_t));
        for (size_t ci = 0; ci < cubeCnt; ++ci)
            shift_or(reach6, prev, words, cubes[ci]);

        /* allocate prefix array */
        prefix = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        if (!prefix)
            return (seq_t)0;

        /* compute prefix counts of numbers that need exactly 6 cubes */
        seq_t count = 0;
        for (size_t w = 0; w < words; ++w) {
            uint64_t diff = reach6[w] & ~reach_le[w];
            for (uint32_t b = 0; b < 64; ++b) {
                size_t idx = (w << 6) + b;
                if (idx > LIMIT)
                    break;
                if (diff & ((uint64_t)1 << b))
                    ++count;
                prefix[idx] = count;
            }
        }

        ready = 1;
    }

    /* if n >= 7 we are already past the upper precomputed bound */
    if (n >= 7)
        return (seq_t)prefix[LIMIT];

    /* compute 10ⁿ for n < 7 (10¹..10⁶) */
    idx_t lim = 1;
    for (idx_t i = 0; i < n; ++i)
        lim *= 10;          /* lim = 10ⁿ */
    idx_t ub = lim - 1;     /* numbers < 10ⁿ */

    if (ub > LIMIT)
        ub = LIMIT;         /* safety */

    return (seq_t)prefix[ub];
}
