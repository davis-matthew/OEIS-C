/*
 * A096913  Numbers k such that (largest digit of k)^(smallest digit of k) + k
 *          is a square.
 * https://oeis.org/A096913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 967 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096913 – Numbers k such that (largest digit)^(smallest digit) + k is a square. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;           /* dynamic array of known terms */
static size_t cache_size = 0;         /* number of cached terms   */
static size_t cache_cap   = 0;         /* allocated capacity       */
static idx_t next_candidate = 0;      /* next k to test           */

/* Append a new term to the cache, expanding storage as necessary. */
static void append_term(seq_t val)
{
    if (cache_size == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    cache[cache_size++] = val;
}

/* Test whether a given integer k satisfies the sequence condition. */
static bool is_valid_k(idx_t k)
{
    /* Find largest and smallest decimal digits of k */
    int largest = 0, smallest = 9;
    idx_t tmp = k;
    if (tmp == 0) {                /* special case: k==0 */
        largest = smallest = 0;
    } else {
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            if (d > largest) largest = d;
            if (d < smallest) smallest = d;
            tmp /= 10;
        }
    }

    /* Compute largest^smallest (0^0 → 1) */
    seq_t pow_val = 1;
    if (!(largest == 0 && smallest == 0)) { /* zero^0 already 1 */
        seq_t base = largest;
        for (int i = 0; i < smallest; ++i)
            pow_val *= base;
    }

    /* Check if pow_val + k is a perfect square */
    seq_t sum = pow_val + (seq_t)k;
    seq_t root = floor_sqrt(sum);
    return root * root == sum;
}

/* Generate terms until the cache contains at least n entries.  */
static void generate_until(idx_t n)
{
    while (cache_size < n) {
        if (is_valid_k(next_candidate))
            append_term((seq_t)next_candidate);
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, but guard anyway */
    generate_until(n);
    return cache[n - 1];
}
