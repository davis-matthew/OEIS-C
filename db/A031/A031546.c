/*
 * A031546  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 48.
 * https://oeis.org/A031546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031546: Numbers k such that the continued fraction for sqrt(k)
 * has even period and the central term of the periodic part,
 * after deleting the last term, equals 48. */

#include <stdbool.h>
#include <stdlib.h>

/* integer square root for 64‑bit numbers (floor) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid == 0) break;
        if (mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* check whether D satisfies the condition */
static inline bool is_valid(idx_t D)
{
    if (D <= 1) return false;

    idx_t a0 = isqrt_u64(D);
    if (a0 * a0 == D) return false;          /* perfect square */

    /* first pass: find period length */
    idx_t m = 0, d = 1, a = a0;
    idx_t len = 0;
    do {
        __int128 tmp = (__int128)d * a - m;
        m = (idx_t)tmp;
        tmp = (__int128)D - (__int128)m * m;
        d = (idx_t)(tmp / d);
        a = (a0 + m) / d;
        len++;
    } while (a != 2 * a0);

    /* even period required */
    if ((len & 1) == 0) {
        idx_t mid = (len - 1) / 2;   /* central index after deletion */

        /* second pass: generate up to mid */
        m = 0; d = 1; a = a0;
        idx_t term = 0;
        for (idx_t i = 0; i <= mid; ++i) {
            __int128 tmp = (__int128)d * a - m;
            m = (idx_t)tmp;
            tmp = (__int128)D - (__int128)m * m;
            d = (idx_t)(tmp / d);
            a = (a0 + m) / d;
            term = a;
        }
        return term == 48;
    }
    return false;
}

/* cache of found terms */
static seq_t *cached = NULL;
static size_t cached_cap = 0;
static idx_t cached_cnt = 0;
static idx_t next_k = 2;   /* next integer to test */

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, avoid UB */

    while (cached_cnt < n) {
        if (is_valid(next_k)) {
            /* store the term */
            if (cached_cnt == cached_cap) {
                size_t new_cap = cached_cap ? cached_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
                if (!tmp) { free(cached); return (seq_t)0; }
                cached = tmp; cached_cap = new_cap;
            }
            cached[cached_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cached[n - 1];
}
