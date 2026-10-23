/*
 * A194737  Number of k such that {-k*sqrt(2)} > {-n*sqrt(2)}, where { } =
 *          fractional part.
 * https://oeis.org/A194737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 279, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194737
 *
 *  a(n) = #{ k | 1 ≤ k ≤ n  and  { k * √2 } < { n * √2 } }
 *
 *  {x} denotes the fractional part of x.
 *
 *  The implementation pre‑computes the fractional parts of k*√2
 *  scaled by 2^56, which provides more than enough precision for
 *  n ≤ 10000.  The values are stored as uint64_t.  When a new
 *  n larger than any previously seen is requested, the function
 *  extends the tables and computes the required a(n) by a simple
 *  linear scan.  All calculations are performed with integer
 *  arithmetic; no floating point is used.
 */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t SCALE = (UINT64_C(1) << 56);   /* 2^56 */

/* 64‑bit value floor( √2 * SCALE ).  Computed once.  */
static uint64_t SQRT2_SCALED = 0;

/* Tables: fractional parts and memoised sequence values.  */
static uint64_t *frac = NULL;   /* frac[k] = fractional part of k*√2, scaled */
static seq_t  *memo = NULL;     /* memo[n] = a(n) */
static idx_t   max_cached = 0;

/* Compute floor( √2 * SCALE ) using integer arithmetic. */
static void init_sqrt2_scaled(void)
{
    if (SQRT2_SCALED != 0)
        return;

    /* √2 * SCALE = √(2) * 2^56 = √(2^(113)) */
    const unsigned __int128 target = ((unsigned __int128)1) << 113;     /* 2^113 */
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 57;  /* upper bound for the result */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= target)
            low = mid;
        else
            high = mid - 1;
    }
    SQRT2_SCALED = (uint64_t)low;
}

/* Extend the tables to include all indices up to n. */
static void extend_tables(idx_t n)
{
    if (n <= max_cached)
        return;

    init_sqrt2_scaled();

    /* Allocate or grow fractional part table. */
    if (!frac) {
        frac = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        if (!frac) { abort(); }
    } else {
        uint64_t *tmp = (uint64_t *)realloc(frac, (n + 1) * sizeof(uint64_t));
        if (!tmp) { abort(); }
        frac = tmp;
    }

    /* Allocate or grow memoised a(n) array. */
    if (!memo) {
        memo = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!memo) { abort(); }
    } else {
        seq_t *tmp = (seq_t *)realloc(memo, (n + 1) * sizeof(seq_t));
        if (!tmp) { abort(); }
        memo = tmp;
    }

    /* Fill in new fractional parts and compute a(i) for i > max_cached. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        unsigned __int128 prod = (unsigned __int128)i * SQRT2_SCALED;
        frac[i] = (uint64_t)(prod % SCALE);

        /* Count how many previous fractional parts are smaller. */
        uint64_t count = 0;
        for (idx_t j = 1; j < i; ++j)
            if (frac[j] < frac[i])
                ++count;
        memo[i] = (seq_t)count;
    }

    max_cached = n;
}

/*  Public function required by the benchmark harness. */
seq_t a(idx_t n)
{
    if (n == 0)       /* the sequence is defined for n ≥ 1, but
                         we return 0 for n = 0 to be harmless. */
        return (seq_t)0;

    extend_tables(n);
    return memo[n];
}
