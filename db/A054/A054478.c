/*
 * A054478  Numbers not divisible by any of their digits when written in base
 *          6.
 * https://oeis.org/A054478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   numbers not divisible by any of their digits when written in
 *            base 6
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if val has a non‑zero base‑6 digit that divides val. */
static bool has_bad_divisor(seq_t val)
{
    seq_t tmp = val;
    while (tmp > 0) {
        seq_t q   = tmp / 6;
        seq_t rem = tmp - q * 6;          /* base‑6 digit (0 … 5) */
        if (rem != 0) {
            seq_t div = val / rem;
            if ((val - div * rem) == 0)
                return true;              /* this digit divides val */
        }
        tmp = q;
    }
    return false;
}

/* a(n) – nth term of A054478 */
seq_t a(idx_t n)
{
    /* cache of already computed terms (zero‑based) */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 1;     /* next integer to test */

    /* already have enough terms? */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* initialise cache if needed */
    if (!cache) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(1);
    }

    /* extend cache until we reach n terms */
    while (cache_len < n) {
        seq_t cand = last_checked;
        while (has_bad_divisor(cand))   /* skip bad numbers */
            cand = cand + 1;

        if (cache_len >= cache_cap) {  /* grow cache */
            cache_cap <<= 1;
            cache = mem_resize(seq_t, cache, cache_cap);
            if (!cache) exit(1);
        }

        cache[cache_len++] = cand;
        last_checked = cand + 1;
    }

    return cache[n - 1];
}
