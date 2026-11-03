/*
 * A201879  Numbers n such that sigma_2(n) - n^2 is a square.
 * https://oeis.org/A201879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 2604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9765 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201879: numbers n such that the sum of squares of proper divisors of n is a square.
   Computation is performed by testing integers in ascending order and recording the ones
   that satisfy the condition. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

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

/* Cache of the sequence values found so far. */
static seq_t *seq_vals = NULL;   /* array of sequence terms */
static size_t seq_len = 0;       /* number of terms stored */
static size_t seq_cap = 0;       /* current capacity of seq_vals */
static idx_t  last_tested = 1;   /* next integer to test */

/* Append a term to the cache, growing the array if necessary. */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *new_arr = mem_resize(seq_t, seq_vals, new_cap);
        /* realloc may return NULL only if out of memory; the build environment
           is expected to have sufficient memory for this task. */
        seq_vals = new_arr;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Check whether integer n satisfies: sum of squares of proper divisors of n is a square. */
static bool is_good(idx_t n)
{
    /* sigma_k returns the sum of the k-th powers of all divisors. */
    seq_t sigma_sq = sigma_k(n, (uint8_t)2);   /* sum of squares of all divisors */
    seq_t nn        = (seq_t)n;
    seq_t delta     = sigma_sq - nn * nn;      /* sum of squares of proper divisors */

    seq_t root      = floor_sqrt(delta);
    return (root * root == delta);
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; a(0) is irrelevant */

    /* Generate sequence terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        if (is_good(last_tested))
            seq_append((seq_t)last_tested);
        ++last_tested;
    }

    return seq_vals[(size_t)n - 1];
}
