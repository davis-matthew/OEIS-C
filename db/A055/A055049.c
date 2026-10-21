/*
 * A055049  Numbers of the form 5^(2*i+1)*(5*j+-2).
 * https://oeis.org/A055049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(2*i+1)*(5*j±2), n ≥ 1, starting at a(1)=10
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by merging all arithmetic progressions   */
/* 10·25^i + k·25^(i+1) and 15·25^i + k·25^(i+1) (k ≥ 0).          */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */

    /*------------------------------------------------------------------*/
    /* Build a small list of arithmetic streams.                        */
    /*------------------------------------------------------------------*/
    /* Preallocate a few entries – more than enough for n up to ~1e5.      */
    /* Each i contributes two streams ( +2 and –2 ).                    */
    /* All arithmetic is done in idx_t (64‑bit) to avoid overflow.       */
    /*------------------------------------------------------------------*/
    const size_t MAX_STREAMS = 64;
    idx_t val[MAX_STREAMS];
    idx_t step[MAX_STREAMS];
    size_t stream_cnt = 0;

    idx_t pow25 = 1;                 /* 25^i                               */
    while (stream_cnt + 2 <= MAX_STREAMS) {
        /* first stream for i: start = 10·25^i, step = 25^(i+1) */
        idx_t start1 = 10 * pow25;
        idx_t stp   = 25 * pow25;
        val[stream_cnt]     = start1;
        step[stream_cnt++]  = stp;

        /* second stream for i: start = 15·25^i, step = 25^(i+1) */
        idx_t start2 = 15 * pow25;
        val[stream_cnt]     = start2;
        step[stream_cnt++]  = stp;

        /* stop if multiplying by 25 would overflow 64‑bit idx_t       */
        if (pow25 > (UINT64_MAX / 25))
            break;
        pow25 *= 25;                /* next 25^i                          */
    }

    /*------------------------------------------------------------------*/
    /* Generate the first n terms by repeatedly taking the smallest    */
    /* current entry and advancing it by its common difference.          */
    /*------------------------------------------------------------------*/
    idx_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        size_t best = 0;
        idx_t   best_val = val[0];
        for (size_t i = 1; i < stream_cnt; ++i) {
            if (val[i] < best_val) {
                best_val = val[i];
                best = i;
            }
        }
        result = best_val;          /* this is the next term          */
        val[best] += step[best];    /* advance this stream             */
    }
    return (seq_t)result;
}
