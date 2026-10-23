/*
 * A159619  Slowest increasing sequence beginning with 4 such that n and a(n)
 *          are either both evil or both odious.
 * https://oeis.org/A159619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A159619
 * Sequence: slowest increasing sequence beginning with 4 such that
 *   n and a(n) are both evil or both odious (have the same
 *   parity of the number of 1‑bits in binary).
 *   For each n >= 1 the next term a(n) is the minimal integer > a(n‑1)
 *   with popcount(a(n)) % 2 == popcount(n) % 2.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helpers: popcount for idx_t (uint64_t) and for 64‑bit integers     */
/* ------------------------------------------------------------------ */
static inline unsigned popcount_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned)__builtin_popcountll((unsigned long long)x);
#else
    /* Fallback: Kernighan's algorithm */
    unsigned c = 0;
    uint64_t u = (uint64_t)x;
    while (u) {
        u &= u - 1;
        ++c;
    }
    return c;
#endif
}

static inline unsigned popcount_uint64(uint64_t x)
{
    return popcount_idx((idx_t)x);
}

/* ------------------------------------------------------------------ */
/*  Cached sequence storage                                         */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache      = NULL;   /* array of computed terms      */
static size_t seq_cache_cap  = 0;      /* allocated capacity           */
static idx_t  seq_cache_last = 0;      /* highest index computed (1‑based) */

/* ------------------------------------------------------------------ */
/*  Ensure that terms 1..n (inclusive) are computed and stored.      */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (seq_cache_last >= n)
        return;                      /* already computed */

    /* Increase capacity if necessary */
    size_t needed = (size_t)n;
    if (seq_cache_cap < needed) {
        size_t new_cap = (seq_cache_cap > 0) ? seq_cache_cap : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – abort */
            abort();
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* Initialization for the very first term */
    if (seq_cache_last == 0) {
        seq_cache[0] = (seq_t)4;      /* a(1) = 4 */
        seq_cache_last = 1;
        /* If n == 1 we are done */
        if (n == 1)
            return;
    }

    /* Compute terms from seq_cache_last+1 up to n */
    for (idx_t idx = seq_cache_last + 1; idx <= n; ++idx) {
        uint64_t candidate = (uint64_t)seq_cache[idx - 2] + 1; /* > previous term */
        unsigned parity_n = popcount_idx(idx) & 1;             /* parity of n */
        while ((popcount_uint64(candidate) & 1) != parity_n)
            ++candidate;                                     /* next candidate */
        seq_cache[idx - 1] = (seq_t)candidate;
    }

    seq_cache_last = n;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)         /* safe guard, not used by OEIS */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)4; /* first term */

    ensure_cache(n);
    return seq_cache[n - 1];
}
