/*
 * A133108  Representation of a dense para-sequence.
 * https://oeis.org/A133108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is built in segments. Segment 1: 1,2 (length =
 *            3¹‑1) Segment k+1 for k≥1: each element of segment k is preceded
 *            by two consecutive new numbers, and the segment ends with two
 *            final new numbers. Thus len_k = 3^k − 1. For 1≤i≤len_{k‑1} the
 *            (3i‑2)nd, (3i‑1)st, (3i)th positions of segment...
 * Range:     u8 n <= 377, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static inline seq_t pow3_seq(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) r *= (seq_t)3;
    return r;
}

seq_t a(idx_t n)
{
    /* Locate the segment containing the global index n. */
    idx_t seg = 1;          /* segment number (1‑based)   */
    idx_t len = 2;          /* length of current segment  */
    idx_t start = 1;        /* global index of first element in the current segment */

    while (n > start + len - 1) {
        start += len;                /* start of next segment          */
        seg++;                       /* next segment number            */
        len = 3 * len + 2;           /* len_k = 3*len_{k-1}+2           */
    }

    /* Index within the segment (1‑based). */
    idx_t m = n - start + 1;
    if (seg == 1) {                /* Segment 1 is just 1,2         */
        return (m == 1) ? (seq_t)1 : (seq_t)2;
    }

    /* Length of the previous segment. */
    idx_t len_prev = (len - 2) / 3;   /* (3^k‑1-2)/3 = 3^{k‑1}-1 = len_{k-1} */

    /* Check whether we are in the main body (old element case). */
    if (m <= 3 * len_prev) {
        idx_t pos   = m - 1;          /* 0‑based offset inside segment */
        idx_t block = pos / 3;        /* which element of previous segment */
        idx_t offset = pos % 3;       /* 0 or 1 = new, 2 = old          */

        if (offset == 2) {            /* old element → recurse              */
            /* global index of the corresponding old element in the previous segment */
            idx_t n_prev = start - len_prev + block;
            return a(n_prev);
        } else {                      /* new number                         */
            idx_t new_index = block * 2 + offset;   /* 0‑based index among new numbers  */
            seq_t val = pow3_seq(seg - 1) + new_index;
            return val;
        }
    } else {
        /* The last two positions are the final new numbers of this segment. */
        /* They are the last two entries among the 2*len_prev new numbers. */
        idx_t new_index = 2 * len_prev + (m - (3 * len_prev + 1));
        seq_t val = pow3_seq(seg - 1) + new_index;
        return val;
    }
}
