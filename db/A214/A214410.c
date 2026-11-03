/*
 * A214410  Numbers that can't be expressed as the sum of a square and a
 *          Fibonacci number.
 * https://oeis.org/A214410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that can't be expressed as the sum of a square and a Fibonacci number. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/ipow_i32.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
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

static void grow_seq_arr(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap * 2);
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = seq_realloc(*arr, *cap, new_cap);
    if (!tmp)
        return;               /* fall back to unchanged array on allocation failure */
    *arr  = tmp;
    *cap  = new_cap;
}

/* Fibonacci cache ------------------------------------------------------ */
static seq_t *fibs  = NULL;
static size_t fib_cnt = 0;
static size_t fib_cap = 0;

static void init_fibs(void)
{
    if (fib_cnt > 0)
        return;
    grow_seq_arr(&fibs, &fib_cap, 3);
    fibs[0] = (seq_t)0;
    fibs[1] = (seq_t)1;
    fibs[2] = (seq_t)1;
    fib_cnt = 3;
}

static void ensure_fibs(seq_t limit)
{
    init_fibs();
    while (fibs[fib_cnt - 1] <= limit) {
        grow_seq_arr(&fibs, &fib_cap, fib_cnt + 1);
        fibs[fib_cnt] = fibs[fib_cnt - 1] + fibs[fib_cnt - 2];
        ++fib_cnt;
    }
}

static bool is_fib(seq_t x)
{
    for (size_t i = 0; i < fib_cnt; ++i)
        if (fibs[i] == x)
            return true;
    return false;
}

/* Test whether m can be written as square + Fibonacci. ----------------- */
static bool can_express(seq_t m)
{
    seq_t sq_max = floor_sqrt(m);
    idx_t smax = (idx_t)sq_max;          /* safe for the ranges we need */
    for (idx_t s = smax;; --s) {
        seq_t sq = (seq_t)s * (seq_t)s;
        if (sq > m)
            continue;                    /* should not happen, but stay safe */
        seq_t rem = m - sq;
        if (is_fib(rem))
            return true;
        if (s == 0)
            break;
    }
    return false;
}

/* Missing numbers cache --------------------------------------------------- */
static seq_t *missing = NULL;
static size_t miss_cnt = 0;
static size_t miss_cap = 0;
static idx_t last_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined; return 0 as a safe fallback */
        return (seq_t)0;

    while (miss_cnt < n) {
        ++last_checked;
        seq_t cur = (seq_t)last_checked;
        ensure_fibs(cur);
        if (!can_express(cur)) {
            grow_seq_arr(&missing, &miss_cap, miss_cnt + 1);
            missing[miss_cnt] = cur;
            ++miss_cnt;
        }
    }
    return missing[n - 1];
}
