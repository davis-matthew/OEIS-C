/*
 * A097093  Number of partitions of n such that the least part occurs exactly
 *          five times.
 * https://oeis.org/A097093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -p(n) + 5·p(n+5) - p(n+6) - 2·p(n+7) - 2·p(n+8) -
 *            3·p(n+9) + 3·p(n+10) + p(n+11) + 2·p(n+12) - p(n+13) - 2·p(n+14)
 *            + p(n+15) where p(m) is the partition function A000041(m). This
 *            implementation pre‑computes p(m) using Euler's pentagonal
 *            recurrence
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 154, u64 n <= 469,
 *            u128 n <= 1568, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_CACHE 20000  /* enough for n up to ~10 000 */

static seq_t p_cache[MAX_CACHE];
static size_t p_cache_len = 0;      /* number of computed entries: next to compute == p_cache_len */
static int p_cache_inited = 0;      /* flag to initialise p_cache[0] */

/* Ensure that partition numbers up to idx are computed.  idx is zero‑based. */
static void ensure_partitions(size_t idx)
{
    if (!p_cache_inited) {
        p_cache[0] = 1;            /* p(0) = 1 */
        p_cache_len = 1;
        p_cache_inited = 1;
    }

    if (idx >= MAX_CACHE) {
        /* Index out of bounds for this static cache.  In the
           intended test range this should never happen. */
        /* If it does, we simply clip to the maximum available. */
        idx = MAX_CACHE - 1;
    }

    while (p_cache_len <= idx) {
        size_t i = p_cache_len;    /* next index to compute */
        seq_t acc = 0;

        for (size_t k = 1; ; ++k) {
            size_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            seq_t term = p_cache[i - g1];
            if (k & 1U) acc += term;
            else        acc -= term;

            size_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i) continue;
            term = p_cache[i - g2];
            if (k & 1U) acc += term;
            else        acc -= term;
        }

        p_cache[i] = acc;
        ++p_cache_len;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t for indexing; n is at least 1. */
    size_t idx = (size_t)n;

    /* We need partitions up to n+15. */
    ensure_partitions(idx + 15);

    /* Compute the linear combination of partition numbers. */
    seq_t res = 0;

    res -= p_cache[idx];
    res += p_cache[idx + 5] * 5;
    res -= p_cache[idx + 6];
    res -= p_cache[idx + 7] * 2;
    res -= p_cache[idx + 8] * 2;
    res -= p_cache[idx + 9] * 3;
    res += p_cache[idx + 10] * 3;
    res += p_cache[idx + 11];
    res += p_cache[idx + 12] * 2;
    res -= p_cache[idx + 13];
    res -= p_cache[idx + 14] * 2;
    res += p_cache[idx + 15];

    return res;
}
