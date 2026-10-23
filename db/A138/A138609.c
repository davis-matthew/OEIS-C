/*
 * A138609  List the first term from A042963, then 2 terms from A014601
 *          (starting from 3), 3 terms from A042963, 4 terms from A014601,
 *          etc.
 * https://oeis.org/A138609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is produced by concatenating blocks of odd size taken from
 *            A042963 (1,2,5,6,9,10,…) and even size taken from A014601
 *            (3,4,7,8,11,12,…) in increasing order. The block with index t
 *            (t≥1) has t elements; the first block (t=1) contains the first
 *            element of A042963. For each n, find the block tha...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   term_A014601, term_A042963
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/term_A014601.h"
#include "../utils/term_A042963.h"

/* Determine block t such that t*(t+1)/2 >= n. */
static inline idx_t find_block(idx_t n)
{
    idx_t t = 1;
    while ((__uint128_t)t * (t + 1) / 2 < n)
        ++t;
    return t;
}

seq_t a(idx_t n)
{
    if (n < 1)                /* safe fallback for out of bounds */
        return (seq_t)0;

    idx_t t = find_block(n);               /* block number */
    idx_t prev_total = t * (t - 1) / 2;     /* terms before this block */
    idx_t pos = n - prev_total;            /* 1‑based position in this block */

    if (t & 1) { /* odd block – use A042963 */
        idx_t m   = (t + 1) / 2;           /* count of odd blocks up to t */
        idx_t prev = (m - 1) * (m - 1);    /* terms from A042963 before this block */
        idx_t k = prev + pos;              /* index in A042963 for this term */
        return term_A042963(k);
    } else { /* even block – use A014601 */
        idx_t m   = t / 2;                 /* count of even blocks up to t */
        idx_t prev = (m - 1) * m;          /* terms from A014601 before this block */
        idx_t k = prev + pos;              /* index in A014601 for this term */
        return term_A014601(k);
    }
}
