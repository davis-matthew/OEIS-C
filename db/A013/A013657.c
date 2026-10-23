/*
 * A013657  Period of continued fraction for sqrt(n) contains more than one 1,
 *          but no two consecutive 1's.
 * https://oeis.org/A013657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for integers m whose continued fraction of sqrt(m)
   has a period containing more than one ‘1’ and no consecutive ‘1’s. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root, floor(sqrt(n)). */
static inline idx_t floor_sqrt_uint64(idx_t n)
{
    idx_t left = 0, right = ((idx_t)1ULL << 32);
    while (left < right) {
        idx_t mid = (left + right + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) left = mid;
        else right = mid - 1;
    }
    return left;
}

/* Test whether a given integer n satisfies the required property. */
static bool good_candidate(idx_t n)
{
    idx_t s = floor_sqrt_uint64(n);
    if ((unsigned __int128)s * s == n)             /* skip perfect squares */
        return false;

    idx_t m = 0, d = 1;
    idx_t a0 = s;
    idx_t a = a0;
    bool prev_one = false;
    int count_one = 0;
    bool invalid = false;

    for (;;) {
        unsigned __int128 prod = (unsigned __int128)d * a;
        m = (idx_t)(prod - m);

        unsigned __int128 dif = (unsigned __int128)n - (unsigned __int128)m * m;
        d = (idx_t)(dif / d);

        a = (a0 + m) / d;

        if (a == 1) {                /* count the 1's and check for adjacency */
            count_one++;
            if (prev_one) invalid = true;
            prev_one = true;
        } else
            prev_one = false;

        if (a == 2 * a0)              /* period terminates */
            break;
    }
    return (!invalid && count_one > 1);
}

/* Static storage for the list of good numbers. */
static idx_t *good = NULL;
static size_t good_len = 0, good_cap = 0, next_candidate = 1;

/* Grow the dynamic array when needed. */
static void grow_good(size_t need)
{
    if (good_cap >= need)
        return;
    size_t new_cap = good_cap ? good_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(good, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    good = tmp;
    good_cap = new_cap;
}

/* Ensure that enough good numbers have been computed to reach the target. */
static void ensure_good(idx_t target)
{
    if (target <= good_len) return;
    while (good_len < target) {
        if (good_candidate(next_candidate)) {
            grow_good(good_len + 1);
            good[good_len++] = next_candidate;
        }
        next_candidate++;
    }
}

/* Public API: return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    ensure_good(n);
    return (seq_t)good[n - 1];  /* a(1) is the first element */
}
