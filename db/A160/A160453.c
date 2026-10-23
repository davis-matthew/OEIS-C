/*
 * A160453  Numbers k which have a prime divisor p such that 1 is the only
 *          positive integer which divides k/p^m and is congruent to 1 modulo
 *          p, where p^(m+1) does not divide k.
 * https://oeis.org/A160453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 228, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, has_div1_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A160453.
   Numbers k with a prime divisor p such that
   1 is the only positive integer dividing k/p^m
   and congruent to 1 modulo p, where p^(m+1) does not divide k.
   The routine searches k successively, memoising the sequence
   up to the requested index. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/has_div1_rec.h"

/* Return true iff t has any divisor >1 that is ≡ 1 (mod p). */
static bool has_div1(uint64_t t, uint64_t p)
{
    if (t <= 1)
        return false;
    /* factor t: use at most 64 prime factors (safe for 64‑bit). */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(t, pr, ex, &cnt);
    return has_div1_rec(pr, ex, cnt, 0, 1, p);
}

/* Test whether k satisfies the defining property. */
static bool qualifies(uint64_t k)
{
    if (k == 1)
        return true;   /* 1 is always in the sequence */

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(k, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        uint64_t p_power = 1;
        for (uint64_t j = 0; j < ex[i]; ++j)
            p_power *= p;
        uint64_t t = k / p_power;

        if (t == 1)            /* k = p^m */
            return true;
        if (!has_div1(t, p))   /* this p works */
            return true;
    }
    return false;   /* no suitable prime divisor found */
}

/* Public function returning the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t last_checked = 0;

    /* initialise cache lazily */
    if (!cache)
    {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (len < n)
    {
        ++last_checked;
        if (qualifies(last_checked))
        {
            if (len == cap)
            {
                size_t new_cap = cap * 2;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                memcpy(new_cache, cache, len * sizeof(seq_t));
                free(cache);
                cache = new_cache;
                cap = new_cap;
            }
            cache[len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
