/*
 * A008888  Aliquot sequence starting at 138.
 * https://oeis.org/A008888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   * a(0) = 138 * a(n) = sum of proper divisors of a(n‑1) for n > 0
 *            * (i.e. the aliquot sum function A001065 applied repeatedly, *
 *            the sequence stops after reaching 0, which occurs at n = 178)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 110, u64 n <= 178,
 *            u128 n <= 178, big any n
 * Validated: full, pass, 179 terms through n = 178 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* A short helper: aliquot sum for a 64‑bit unsigned integer.
 * The sequence values stay below 2^64 for all steps until 0 is reached,
 * so this helper suffices for every requested n. */
static seq_t aliquot_sum_uint64(uint64_t x)
{
    if (x <= 1) return 0;          /* 0 and 1 have no proper divisors */
    uint64_t sum = 1;              /* 1 is always a proper divisor */
    for (uint64_t i = 2; i <= x / i; ++i) {
        if (x % i == 0) {
            sum += i;
            uint64_t j = x / i;
            if (j != i) sum += j;   /* avoid adding a square root twice */
        }
    }
    return (seq_t)sum;             /* cast to the build‑specific seq_t */
}

/* Cache for the first 179 terms (0 … 178).  After 0 is reached
 * all subsequent terms stay 0, so no larger array is required. */
static seq_t  cache[179];
static size_t cache_len = 0;   /* number of entries already computed */

/* Main entry point required by the OEIS build system. */
seq_t a(idx_t n)
{
    /* For indices ≥ 179 the sequence is already 0. */
    if (n >= 179) return 0;

    size_t idx = (size_t)n;

    /* Compute missing terms up to idx, stopping early if 0 is reached. */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)138;
        } else {
            uint64_t prev = (uint64_t)cache[i - 1];
            cache[i] = aliquot_sum_uint64(prev);
        }

        /* Once 0 occurs, all following terms are 0 as well. */
        if (cache[i] == (seq_t)0) {
            /* Fill remaining entries up to idx with 0 to keep cache_len
             * in sync for subsequent calls. */
            for (size_t j = i + 1; j <= idx; ++j) cache[j] = 0;
            cache_len = idx + 1;   /* all computed up to idx */
            return cache[idx];
        }
    }

    cache_len = idx + 1;   /* mark the range now known */
    return cache[idx];
}
