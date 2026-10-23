/*
 * A141402  Triangle T(n, k) = n^2 + (2*k*(n-k))^2, read by rows.
 * https://oeis.org/A141402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) and k = n -
 *            r*(r+1)/2, T(r,k) = r^2 + (2*k*(r-k))^2
 * Range:     u8 n <= 7, u16 n <= 285, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS A141402 sequence, which is the
   triangle T(n,k)=n^2 + (2*k*(n-k))^2 read by rows.  */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arbitrary‑precision arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* r = floor((sqrt(8*n + 1) - 1) / 2) */
    seq_t sq = floor_sqrt(n_seq * (seq_t)8 + (seq_t)1);
    idx_t r = (idx_t)((sq - (seq_t)1) / (seq_t)2);

    /* offset = r*(r+1)/2  (first index of row r) */
    seq_t offset = ((seq_t)r * ((seq_t)r + (seq_t)1)) / (seq_t)2;

    /* k = n - offset */
    seq_t k_seq = n_seq - offset;

    /* Compute T(r,k) = r^2 + (2*k*(r-k))^2 */
    seq_t r_seq   = (seq_t)r;
    seq_t diff    = r_seq - k_seq;          /* r - k */
    seq_t part1   = r_seq * r_seq;           /* r^2 */
    seq_t mult    = k_seq * diff;            /* k * (r-k) */
    mult         *= (seq_t)2;                /* 2*k*(r-k) */
    seq_t part2   = mult * mult;             /* (2*k*(r-k))^2 */

    return part1 + part2;
}
