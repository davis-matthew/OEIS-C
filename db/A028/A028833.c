/*
 * A028833  Index of redundancy in period of continued fraction for sqrt(n).
 * https://oeis.org/A028833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A003285(n) – A028832(n) where A003285(n) is the period
 *            length of the continued fraction expansion of √n and A028832(n)
 *            is the number of distinct terms in that period. The function
 *            below computes the period and the unique count directly for any
 *            n ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
/* --------- integer square‑root for uint64_t --------- */
static inline idx_t isqrt_u64(idx_t n)
{
    /* use long double to get a good starting guess */
    idx_t r = (idx_t)sqrt((long double)n);
    /* adjust upwards */
    while (( (__int128)(r + 1) * (r + 1) ) <= n) ++r;
    /* adjust downwards */
    while (( (__int128)r * r ) > n) --r;
    return r;
}

/* ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* perfect square: period length is 0, so answer is 0 */
    idx_t a0 = isqrt_u64(n);
    if (a0 * a0 == n)
        return 0;

    /* continued fraction variables */
    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a_val = a0;

    idx_t period  = 0;          /* number of terms in the period   */
    idx_t unique  = 0;          /* number of distinct period terms */

    /* store whether we've seen a value of a_next */
    size_t seen_size = (size_t)(2 * a0 + 1);
    bool *seen = (bool *)calloc(seen_size, sizeof(bool));
    if (!seen)   /* fall back if allocation fails */
        return 0;

    for (;;)
    {
        /* next iteration */
        idx_t m_next = d * a_val - m;
        /* (n - m_next*m_next) fits in 128 bits; divide by d */
        __int128 tmp    = (__int128)n - (__int128)m_next * m_next;
        idx_t  d_next   = (idx_t)(tmp / d);
        idx_t  a_next   = (a0 + m_next) / d_next;

        period++;
        if (!seen[(size_t)a_next])
        {
            seen[(size_t)a_next] = true;
            unique++;
        }
        if (a_next == 2 * a0)      /* terminating condition */
            break;

        /* update state */
        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    free(seen);
    return (seq_t)(period - unique);
}
