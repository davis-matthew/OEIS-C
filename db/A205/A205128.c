/*
 * A205128  Ordered differences of distinct hexagonal numbers.
 * https://oeis.org/A205128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A205128 – Ordered differences of distinct hexagonal numbers
 *
 *  The sequence consists of all differences s(j) – s(i) where
 *  s(k) = k(2k – 1) are the hexagonal numbers, j > i ≥ 1,
 *  sorted by increasing j and, for each j, by increasing i.
 *
 *  The nth term a(n) is given by:
 *
 *      let j be the smallest integer ≥ 2 with C(j) ≥ n,
 *          where C(j) = (j – 1)·j/2   (the count of differences up to index j).
 *      let i = n – C(j – 1)   1‑based within the block for j.
 *      a(n) = H_j – H_i  with  H_k = k(2k – 1).
 */

#include <stdint.h>

static inline idx_t C(idx_t j)
{
    /* Return (j-1)*j/2 without overflow for the ranges needed. */
    return (idx_t)(((__int128)(j - 1) * j) / 2);
}

seq_t a(idx_t n)
{
    /* Quick guard for the offset (n ≥ 1 by specification). */
    if (n == 0) return 0;

    idx_t low = 2, high = 2;
    /* Grow the search range until C(high) ≥ n. */
    while (C(high) < n) {
        high = high << 1;                /* double */
        if (high == 0) high = ~ (idx_t)0;/* protect against overflow */
    }

    /* Binary search for the smallest j with C(j) ≥ n. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (C(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t j = low;                       /* j ≥ 2 */
    idx_t prev = C(j - 1);                /* C(j-1) = (j-2)(j-1)/2 */
    idx_t offset = n - prev - 1;          /* 0‑based index within block j */
    idx_t i = offset + 1;                 /* 1‑based i, 1 ≤ i < j */

    seq_t h_j = (seq_t)j * ((seq_t)2 * j - 1);
    seq_t h_i = (seq_t)i * ((seq_t)2 * i - 1);
    return h_j - h_i;
}
