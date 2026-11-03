/*
 * A110208  1 + sum of first n semiprimes.
 * https://oeis.org/A110208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sum of first n semiprimes * (a(0) = 1, a(n) = 1 +
 *            A062198(n) for n>0)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* ---------- Static state ---------- */
static bool state_initialized = false;
static seq_t *sum_arr   = NULL;   /* sum_arr[k] = sum of first k semiprimes */
static idx_t  sum_size  = 0;      /* number of elements stored (>=1)   */
static idx_t  sum_cap   = 0;      /* current allocated capacity         */
static uint64_t next_candidate = 2;   /* next integer to test for semiprime */

/* ---------- Utility: ensure sum array capacity ---------- */
static void ensure_sum_capacity(idx_t need)
{
    if (need + 1 <= sum_cap)
        return;
    idx_t new_cap = sum_cap ? sum_cap << 1 : 16;
    while (new_cap <= need + 1)
        new_cap <<= 1;
    seq_t *new_arr = seq_malloc(new_cap);
    for (idx_t i = 0; i < sum_size; ++i)
        new_arr[i] = sum_arr[i];
    mem_free(sum_arr);
    sum_arr = new_arr;
    sum_cap = new_cap;
}

/* ---------- Initialize static structures ---------- */
static void ensure_state(void)
{
    if (state_initialized)
        return;
    state_initialized = true;

    sum_cap = 16;
    sum_arr = seq_malloc(sum_cap);
    sum_arr[0] = (seq_t)0;  /* sum of zero semiprimes */
    sum_size = 1;
    next_candidate = 2;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Prepare internal state if needed */
    ensure_state();

    /* If we do not yet have sums up to n, extend the array */
    while (sum_size <= n) {
        /* Advance until the next semiprime is found */
        while (!is_semiprime_uint64(next_candidate))
            ++next_candidate;
        /* Append cumulative sum */
        ensure_sum_capacity(sum_size);
        sum_arr[sum_size] = sum_arr[sum_size - 1] + (seq_t)next_candidate;
        ++sum_size;
        ++next_candidate;
    }

    /* Return a(n) = 1 + sum of first n semiprimes */
    return (seq_t)1 + sum_arr[n];
}
