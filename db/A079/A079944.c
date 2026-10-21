/*
 * A079944  A run of 2^n 0's followed by a run of 2^n 1's, for n=0, 1, 2, ...
 * https://oeis.org/A079944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log2(4*(n+2)/3)) - floor(log2(n+2))
 * Range:     u8 n <= 313, u16 n <= 1021, u32 n <= 1021, u64 n <= 1021,
 *            u128 n <= 1021, big any n
 * Helpers:   floor_log2_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_seq_t.h"
/*  For each index n, a(n) equals 0 if n lies in a zero‑run, otherwise 1. */

seq_t a(idx_t n)
{
    /* v = n + 2  (always ≥ 2 for n ≥ 0) */
    seq_t v = (seq_t)n + 2;

    /* s = floor(log2(v))  */
    unsigned s = floor_log2_seq_t(v);

    /* pair index k = floor(log2(v)) – 1   (k ≥ 0) */
    unsigned k = s - 1;

    /* 2^k and 2^(k+1) */
    seq_t powk = ((seq_t)1) << k;   /* 2^k   */
    seq_t two_pok1 = powk << 1;     /* 2^(k+1) */

    /* offset within the current pair (references runs of length 2^k) */
    seq_t offset = v - two_pok1;  /* 0 ≤ offset < 2^k + 2^k */

    /* In the pair the first 2^k positions are zeros, the next 2^k are ones. */
    return (offset < powk) ? (seq_t)0 : (seq_t)1;
}
