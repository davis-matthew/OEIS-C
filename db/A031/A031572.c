/*
 * A031572  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 74.
 * https://oeis.org/A031572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A031572:
   Numbers k such that the continued fraction for √k has even period,
   and if the last term of the periodic part is deleted, the central
   term equals 74.  This implementation evaluates k sequentially and
   caches the results so that any a(n) can be returned in O(1) amortized
   time. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Floor integer square root of a 64‑bit integer.  */
static idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= (unsigned __int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cache to store found terms.  */
static seq_t *cache      = NULL;   /* array of terms   */
static size_t cache_sz   = 0;      /* count of terms   */
static size_t cache_cap  = 0;      /* allocated size   */
static idx_t   next_k    = 2;      /* next integer to test */

/* Append a value to the cache, growing if necessary.  */
static void cache_append(seq_t v)
{
    if (cache_sz >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();      /* allocation failure */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = v;
}

/* Return true if k satisfies the condition.  */
static bool ok_k(idx_t k)
{
    idx_t a0 = _floor_isqrt_u64(k);
    if (a0 * a0 == k)  /* skip perfect squares */
        return false;

    /* Find period length.  */
    idx_t m = 0, d = 1, a = a0;
    idx_t len = 0;
    do {
        /* Safely compute next values using 128‑bit intermediates. */
        unsigned __int128 m_new = (unsigned __int128)d * (unsigned __int128)a - m;
        m = (idx_t)m_new;

        unsigned __int128 m_sq = (unsigned __int128)m * (unsigned __int128)m;
        d = (k - (idx_t)m_sq) / d;

        a = (a0 + m) / d;
        ++len;
    } while (a != 2 * a0);

    /* Even period and central term must be 74 after deleting last term. */
    if ((len & 1) == 0) {
        idx_t central_pos = len / 2 - 1;      /* 0‑based index */
        /* Re‑run to obtain the central partial quotient. */
        m = 0; d = 1; a = a0;
        for (idx_t pos = 0; pos <= central_pos; ++pos) {
            unsigned __int128 m_new = (unsigned __int128)d * (unsigned __int128)a - m;
            m = (idx_t)m_new;
            unsigned __int128 m_sq = (unsigned __int128)m * (unsigned __int128)m;
            d = (k - (idx_t)m_sq) / d;
            a = (a0 + m) / d;
        }
        return a == 74;
    }
    return false;
}

/* Return the n‑th term (n ≥ 1).  */
seq_t a(idx_t n)
{
    while (cache_sz < (size_t)n) {
        if (ok_k(next_k))
            cache_append((seq_t)next_k);
        ++next_k;
    }
    return cache[n - 1];
}
