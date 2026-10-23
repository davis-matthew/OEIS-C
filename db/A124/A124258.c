/*
 * A124258  Triangle whose rows are sequences of increasing and decreasing
 *          squares: 1; 1,4,1; 1,4,9,4,1; ...
 * https://oeis.org/A124258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (A004737(n))², where A004737(n) gives the “row length”
 *            numbers of the symmetric triangle
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 0 ≤ n < 2⁶⁴.
   Uses only integer arithmetic and a single 128‑bit multiply. */
static inline idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;                 /* sqrt(0)=0, sqrt(1)=1 */
    idx_t r = 0;
    for (int i = 31; i >= 0; --i) {      /* bits 31 … 0 cover 0 ≤ r ≤ 2³²‑1 */
        idx_t bit   = (idx_t)1ULL << i;  /* candidate bit */
        idx_t cand  = r | bit;           /* try setting this bit */
        if ((__int128)cand * cand <= n) /* multiplication in 128‑bit */
            r = cand;                   /* keep it if it stays ≤ n */
    }
    return r;
}

seq_t a(idx_t n) {
    /* Assumes n ≥ 1. */
    idx_t r      = isqrt_u64(n - 1) + 1;          /* row number (r ≥ 1) */
    idx_t start  = (r - 1) * (r - 1) + 1;         /* index of row’s first element */
    idx_t offset = n - start;                     /* 0‑based position inside the row */

    seq_t k;
    if (offset < r) {
        /* Ascending part of the row: 1, 2, …, r */
        k = (seq_t)(offset + 1);
    } else {
        /* Descending part of the row: r‑1, …, 1 */
        k = (seq_t)(2 * r - 1 - offset);
    }
    return k * k;
}
