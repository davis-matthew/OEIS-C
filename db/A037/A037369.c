/*
 * A037369  Numbers whose base-7 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A037369 is not given by a simple closed form, but it can be
 *            computed by enumerating positive integers and selecting those
 *            whose decimal digit set and base‑7 digit set are disjoint
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer such that its
   decimal (base‑10) and base‑7 representations share no digits. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity of cache */
static seq_t next_candidate = 1; /* next integer to test */

static inline bool has_no_common_digits(seq_t num)
{
    bool dec_used[10] = {0};
    bool base7_used[7] = {0};
    seq_t tmp;

    if (num == 0)
        return false;            /* 0 uses digit '0' in both bases */

    /* decimal digits */
    tmp = num;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        dec_used[d] = true;
        tmp /= 10;
    }

    /* base‑7 digits */
    tmp = num;
    while (tmp > 0) {
        int d = (int)(tmp % 7);
        base7_used[d] = true;
        tmp /= 7;
    }

    /* check for any common digit */
    for (int d = 0; d <= 6; ++d) {
        if (base7_used[d] && dec_used[d])
            return false;
    }
    return true;
}

static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                 /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index; we need the n‑th term. */
    if (n == 0)
        return 0;                 /* undefined for offset 0, but guard */

    /* generate terms until we have at least n in cache */
    while (cache_len < (size_t)n) {
        ensure_cache_capacity(cache_len + 1);
        if (has_no_common_digits(next_candidate))
            cache[cache_len++] = next_candidate;
        /* always advance candidate to keep monotonically increasing */
        next_candidate++;
    }
    return cache[n - 1];
}
