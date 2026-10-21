/*
 * A220053  Partial sums in rows of A130517, triangle read by rows.
 * https://oeis.org/A220053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 275, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   sum_arith
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A220053 – Partial sums in rows of A130517.
    a(n) is the n-th partial sum T(r,k) of the permutation of 1..r, where
    r is the smallest integer with r(r+1)/2 ≥ n and k = n – r(r−1)/2.
    The permutation is:
      • if r is even: even numbers descending, then odd numbers ascending
      • if r is odd : odd  numbers descending, then even numbers ascending
    T(r,k) = Σ_{i=1..k} p(r,i).                                      */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_arith.h"

/* Find the row r that contains position n (1‑based). */
static inline idx_t
find_row(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1, but safe guard */
    idx_t low = 1, high = n;          /* r ≤ n because 1+2+…+n ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2; /* r(r+1)/2 */
        if ((idx_t)tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/*  Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 1, but safe guard */

    /* Determine row r and position k. */
    idx_t r = find_row(n);
    idx_t prev_tri = ((idx_t)(r - 1) * r) / 2;      /* terms before row r */
    idx_t k = n - prev_tri;                         /* 1‑based in the row */

    /* Length of the descending part of the permutation. */
    idx_t len_desc = (r & 1) ? (r + 1) / 2 : r / 2;  /* ceil(r/2) if odd, else r/2 */

    seq_t result;
    if (k <= len_desc) {
        /* Sum of first k descending terms. */
        seq_t first_desc = (seq_t)r;
        seq_t last_desc  = first_desc - (seq_t)2 * (seq_t)(k - 1);
        result = sum_arith(first_desc, last_desc, k);
    } else {
        /* Sum of all descending terms. */
        seq_t first_desc = (seq_t)r;
        seq_t last_desc  = first_desc - (seq_t)2 * (seq_t)(len_desc - 1);
        seq_t sum_desc   = sum_arith(first_desc, last_desc, len_desc);

        /* Ascending part. */
        idx_t asc_count = k - len_desc;
        seq_t first_asc = (r & 1) ? (seq_t)2 : (seq_t)1;   /* 2 if r odd, 1 if r even */
        seq_t last_asc  = first_asc + (seq_t)2 * (seq_t)(asc_count - 1);
        seq_t sum_asc   = sum_arith(first_asc, last_asc, asc_count);

        result = sum_desc + sum_asc;
    }

    return result;
}
