/*
 * A082385  For each n append T(n), T(T(n)), T^3(n), ..., T^r(n), where T(n) =
 *          A055012(n) and r is the smallest integer such that T^r(n) is one
 *          of the following numbers: 1, 55, 136, 153, 160, 370, 371, 407,
 *          919.
 * https://oeis.org/A082385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = element n of the concatenation of the chains obtained by
 *            * repeatedly applying the function T(k) = sum of the cubes of
 *            the decimal * digits of k, starting with k = 1, 2, 3, …. * For
 *            each start k the chain consists of T(k), T(T(k)), …, * stopping
 *            when the value belongs to the set * S = {1,5...
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: compute T(k) = sum of cubes of decimal digits of k.
 * k is passed as idx_t (unsigned 64‑bit).
 */
static inline seq_t T_idx(idx_t k)
{
    seq_t sum = 0;
    for (idx_t x = k; x > 0; x /= 10) {
        int d = x % 10;
        sum += (seq_t)d*d*d;
    }
    return sum;
}

/* Helper: test whether x is one of the terminating numbers. */
static inline bool is_stop(seq_t x)
{
    static const seq_t stops[] = {1,55,136,153,160,370,371,407,919};
    for (size_t i = 0; i < sizeof(stops)/sizeof(stops[0]); ++i)
        if (x == stops[i]) return true;
    return false;
}

/* Append value v to a dynamic array arr, increasing capacity as needed. */
static void push_value(seq_t **arr, size_t *len, size_t *cap, seq_t v)
{
    if (*len >= *cap) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 4;
        if (newcap < *len + 1) newcap = *len + 1;
        *arr = mem_resize(seq_t, *arr, newcap);
        *cap = newcap;
    }
    (*arr)[(*len)++] = v;
}

/* The cumulative concatenated list of all chain values. */
static seq_t *concatenated = NULL;
static size_t cat_len   = 0;   /* number of values stored          */
static size_t cat_cap   = 0;   /* allocated capacity               */
static idx_t  built_m   = 0;   /* number of start values processed */

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but avoid crash */

    /* Extend the concatenated list until it contains at least n entries. */
    while (cat_len < (size_t)n) {
        /* Process next starting number k = built_m + 1. */
        idx_t k = ++built_m;
        seq_t cur = T_idx(k);
        for (;;) {
            push_value(&concatenated, &cat_len, &cat_cap, cur);
            if (is_stop(cur)) break;
            /* cur is small (≤ 6561), safe to cast to idx_t. */
            cur = T_idx((idx_t)cur);
        }
    }

    return concatenated[n - 1];
}
