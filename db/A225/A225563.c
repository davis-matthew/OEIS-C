/*
 * A225563  Numbers whose totient-trajectory can be partitioned into two sets
 *          with the same sum.
 * https://oeis.org/A225563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4065, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9194 (2026-11-03)
 * Helpers:   mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th element of OEIS sequence A225563.
   The sequence consists of odd integers whose totient‑trajectory
   can be partitioned into two disjoint subsets with equal sum.
   The function computes the trajectory of each odd integer and
   checks the partition property by a recursive subset sum test
   (pruned by suffix sums).  The results are memoised in a static
   array so that calls with arbitrary order of n are fast.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Recursive subset‑sum test with pruning using suffix sums.  */
static bool _subset_sum(const seq_t *values, size_t idx, seq_t target,
                        const seq_t *suffix, size_t sz)
{
    if (target == 0) return true;            /* found a subset */
    if (idx == sz) return false;             /* exhausted all values */
    if (target > suffix[idx]) return false;   /* cannot reach target */
    if (values[idx] <= target) {
        if (_subset_sum(values, idx + 1, target - values[idx], suffix, sz))
            return true;
    }
    return _subset_sum(values, idx + 1, target, suffix, sz);
}

static seq_t *cache      = NULL;   /* cached terms of the sequence    */
static size_t cache_len  = 0;      /* number of cached terms          */
static size_t cache_cap  = 0;      /* current capacity of cache       */
static uint64_t next_candidate = 3;/* next odd integer to test        */
static size_t max_traj_len = 128;  /* sufficient for all trajectories */

/* Ensure cache has room for at least 'needed' elements.  */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Check whether an odd integer m satisfies the partition property.  */
static bool good_candidate(seq_t m)
{
    seq_t traj[max_traj_len];
    size_t sz = 0;
    seq_t cur = m;
    traj[sz++] = cur;                       /* include initial term */
    while (cur != 1) {
        cur = totient_seq(cur);
        if (sz >= max_traj_len) break;       /* safety guard */
        traj[sz++] = cur;
    }
    /* Sum and parity check */
    seq_t sum = 0;
    for (size_t i = 0; i < sz; ++i) sum += traj[i];
    if ((sum & 1) != 0) return false;        /* odd total cannot be split */
    seq_t target = sum >> 1;                  /* half of the sum */

    /* Compute suffix sums for pruning */
    seq_t suffix[max_traj_len + 1];
    suffix[sz] = 0;
    for (size_t i = sz; i > 0; --i)
        suffix[i - 1] = suffix[i] + traj[i - 1];

    return _subset_sum(traj, 0, target, suffix, sz);
}

/* Public function required by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0, harmless */
    while (cache_len < n) {
        /* Search forward for the next odd integer that satisfies the property. */
        while (!good_candidate((seq_t)next_candidate))
            next_candidate += 2;           /* skip even numbers */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = (seq_t)next_candidate;
        ++cache_len;
        next_candidate += 2;
    }
    return cache[n - 1];
}
