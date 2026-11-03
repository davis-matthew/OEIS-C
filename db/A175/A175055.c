/*
 * A175055  a(n) = decimal equivalent of {A033015(n) written in binary, and
 *          each run of 0's reduced in length by one digit, and each run of
 *          1's reduced in length by one digit}.
 * https://oeis.org/A175055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal equivalent of {A033015(n) written in binary, and
 *            each run of 0's reduced in length by one digit, and each run of
 *            1's reduced in length by one digit}
 * Range:     u8 n <= 87, u16 n <= 4179, u32 n <= 10945, u64 n <= 10945,
 *            u128 n <= 10945, big any n
 * Validated: standard, pass, 86 terms through n = 8757 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: from a number m whose binary representation has no 1–bit
   runs, produce the value of a(n) by shrinking each run by one.       */
static seq_t compute_a_from_m(uint64_t m)
{
    if (m == 0) return 0;
    int bits = 0;
    uint64_t tmp = m;
    while (tmp) { bits++; tmp >>= 1; }

    int prev = (m >> (bits - 1)) & 1;
    int run  = 1;
    seq_t res = 0;

    for (int i = bits - 2; i >= 0; --i) {
        int bit = (m >> i) & 1;
        if (bit == prev) {
            ++run;
        } else {
            int outlen = run - 1;
            for (int k = 0; k < outlen; ++k) {
                res = (res << 1) | (seq_t)prev;
            }
            prev = bit;
            run  = 1;
        }
    }
    /* last run */
    int outlen = run - 1;
    for (int k = 0; k < outlen; ++k) {
        res = (res << 1) | (seq_t)prev;
    }
    return res;
}

/* Helper: check whether x has any run of identical bits of length 1.
   Returns true if such a run exists.   */
static bool has_run_single(uint64_t x)
{
    if (x == 0) return false;
    int bits = 0;
    uint64_t tmp = x;
    while (tmp) { bits++; tmp >>= 1; }

    int prev = (x >> (bits - 1)) & 1;
    int run  = 1;
    for (int i = bits - 2; i >= 0; --i) {
        int bit = (x >> i) & 1;
        if (bit == prev) {
            ++run;
        } else {
            if (run == 1) return true;
            prev = bit;
            run  = 1;
        }
    }
    return (run == 1);
}

/* Static cache for computed a(n) values.  All indices are 1‑based. */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap   = 0;
static size_t a_cache_len   = 0;   /* number of stored values */
static uint64_t next_m      = 1;   /* next integer to test for A033015 */

/* Ensure cache has capacity for at least 'needed' elements. */
static void ensure_a_cache_capacity(size_t needed)
{
    if (needed <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    a_cache = mem_resize(seq_t, a_cache, new_cap);
    a_cache_cap = new_cap;
}

/* Main function requested by the problem.  Computes a(n).        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset, behaviour unrestricted */

    /* extend cache up to n */
    while (a_cache_len < (size_t)n) {
        /* find next integer whose binary representation has no run of length 1 */
        while (has_run_single(next_m)) {
            ++next_m;
        }
        /* compute a value from this integer */
        seq_t val = compute_a_from_m(next_m);

        /* store */
        ensure_a_cache_capacity(a_cache_len + 1);
        a_cache[a_cache_len++] = val;

        ++next_m;
    }
    return a_cache[n - 1];   /* cache is 0‑based */
}
