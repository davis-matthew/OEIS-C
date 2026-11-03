/*
 * A048386  Numbers n such that replacing digits d in decimal expansion of n
 *          with d^2 yields a square.
 * https://oeis.org/A048386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n (n ≥ 0) for which replacing each *
 *            decimal digit d of n by d² yields a perfect square. * Example:
 *            2722 → 4 49 4 4 = 44944 = 212²
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *terms = NULL;      /* stored sequence values */
static size_t terms_cap = 0;     /* allocated capacity */
static size_t terms_cnt = 0;     /* number of terms stored */
static idx_t   next_cand = 0;    /* next candidate to test */

/* test whether n satisfies the definition */
static inline bool is_good(idx_t n)
{
    seq_t f = 0;
    unsigned char digs[64];
    size_t len = 0;
    idx_t t = n;

    /* extract digits in reverse order */
    do {
        digs[len++] = t % 10;
        t /= 10;
    } while (t);

    /* build the transformed number from most significant digit */
    for (size_t i = len; i > 0; --i) {
        unsigned d   = digs[i - 1];
        unsigned sq  = d * d;      /* d², fits in unsigned */
        f = f * ((sq < 10) ? 10 : 100);
        f = f + sq;
    }

    /* check if f is a perfect square */
    seq_t s = floor_sqrt(f);
    return (s * s) == f;
}

/* append a new term to the cache, growing storage if necessary */
static void add_term(seq_t val)
{
    if (terms_cnt == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_cnt++] = val;
}

seq_t a(idx_t n)
{
    /* ensure we have generated up to the nth term */
    while (terms_cnt < (size_t)n) {
        if (is_good(next_cand))
            add_term((seq_t)next_cand);
        next_cand++;
    }
    return terms[n - 1];
}
