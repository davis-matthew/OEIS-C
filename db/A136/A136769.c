/*
 * A136769  n! never ends in this many 0's in base 8.
 * https://oeis.org/A136769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136769:  a(n) is the n-th positive integer that does not occur
   as the number of trailing zeros in base 8 of any factorial.
   The exponent of 2 in m! is v₂(m!) = m – popcount₂(m);
   thus the number of trailing zeros in base 8 is floor(v₂(m!)/3).
   We generate the sequence by simulating v₂(m!) and collecting all
   integers that are skipped by jumps greater than 1.  */

/* Computes a(n) in O(n) time, suitable for n ≤ ~10 000. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Assume seq_t and idx_t are defined in the build environment. */

/* Pre‑allocated dynamic array for the missing trailing‑zero counts. */
static seq_t *miss = NULL;
static size_t miss_sz = 0, miss_cap = 0;

/* Track the last processed factorial index (m) and its trailing‑zero count. */
static uint64_t proc_m = 1;        /* we already processed m = 1, v₂(1!) = 0 */
static uint64_t proc_t = 0;        /* floor(v₂(1!)/3) = 0 */

/* Maximum value that can be represented by the result type. */
#if defined(UINT64_MAX)
static const uint64_t seq_t_max_u64 =
    (sizeof(seq_t) == 8) ? UINT64_MAX : (((uint64_t)1 << (sizeof(seq_t) * 8)) - 1);
#else
static const uint64_t seq_t_max_u64 = 0xFFFFFFFFFFFFFFFFULL;
#endif

/* Append a value to the dynamic array, reallocating as necessary. */
static void append_miss(uint64_t val)
{
    /* Only store values that fit into seq_t. */
    if (val > seq_t_max_u64) return;

    if (miss_sz == miss_cap) {
        miss_cap = miss_cap ? miss_cap * 2 : 16;
        miss = (seq_t *)realloc(miss, miss_cap * sizeof(seq_t));
    }
    miss[miss_sz++] = (seq_t)val;
}

/* Ensure that the first n missing values have been generated. */
static void ensure_missing(idx_t n)
{
    while (miss_sz < (size_t)n) {
        /* Advance to next factorial index. */
        ++proc_m;
        /* v₂(proc_m!) = (proc_m - popcount(proc_m)) + proc_t_previous? */
        uint64_t v2 = proc_m - __builtin_popcountll(proc_m);
        uint64_t t_curr = v2 / 3;        /* floor(v₂(m!)/3) */

        if (t_curr > proc_t + 1) {
            /* All integers between proc_t+1 and t_curr-1 are missing. */
            for (uint64_t v = proc_t + 1; v < t_curr; ++v)
                append_miss(v);
        }
        proc_t = t_curr;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* no defined value for offset 0 */
    ensure_missing(n);
    return miss[(size_t)n - 1];
}
