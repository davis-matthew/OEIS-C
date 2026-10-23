/*
 * A031582  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 84.
 * https://oeis.org/A031582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th integer k such that the continued fraction
 * expansion of sqrt(k) has even period and, after deleting the final term
 * (which is always 2·⌊√k⌋), the central term of the remaining period equals
 * 84.  The result is computed by testing successive k and storing the valid
 * ones in a dynamically sized array.  No external utilities are required
 * beyond the standard C library.  The algorithm runs in O(n·√k) time
 * and uses only integer arithmetic. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *vals = NULL;         /* Array of found k values          */
static size_t vals_len = 0;        /* Number of stored values          */
static size_t vals_cap = 0;        /* Current allocated capacity       */
static uint64_t next_k = 1;        /* Next candidate k to test         */

/* Allocate or grow the vals array to at least 'needed' elements. */
static void ensure_vals_capacity(size_t needed)
{
    if (needed <= vals_cap) return;
    size_t new_cap = vals_cap ? vals_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
    vals = new_ptr;
    vals_cap = new_cap;
}

/* Integer square root of a 64‑bit unsigned integer (floor). */
static uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32; /* √(2^64‑1) < 2^32   */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Test whether a given integer k satisfies the A031582 condition. */
static bool test_k(uint64_t k)
{
    uint64_t sqrt_k = int_sqrt_u64(k);
    if (sqrt_k * sqrt_k == k) return false;          /* skip squares */

    const uint64_t a0 = sqrt_k;
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* The period of sqrt(k) cannot exceed a few thousand for 64‑bit k. */
    uint64_t period[20000];
    int len = 0;

    do {
        /* m = d * a - m   (use 128‑bit to avoid overflow) */
        __uint128_t prod = (__uint128_t)d * a;
        m = (uint64_t)(prod - m);

        /* d = (k - m*m) / d   (k - m*m fits in 64‑bit) */
        d = (k - m * m) / d;

        /* a = (a0 + m) / d   */
        a = (a0 + m) / d;

        period[len++] = a;
    } while (a != 2 * a0);          /* last term reaches 2·a0 */

    if (len % 2 == 1) return false;   /* period must be even */

    int mid_idx = (len - 1) / 2;      /* index after deleting last term */
    return period[mid_idx] == 84;
}

/* ---- OEIS A031582 -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but defensive */

    /* Fill the cache until we have n values. */
    while (vals_len < (size_t)n) {
        if (test_k(next_k)) {
            ensure_vals_capacity(vals_len + 1);
            vals[vals_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return vals[n - 1];
}
