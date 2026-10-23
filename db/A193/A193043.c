/*
 * A193043  Listed by antidiagonals, array A(k,n) = digital root of n-th
 *          nonzero k-gonal number.
 * https://oeis.org/A193043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   min_x
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A193043
 *  Formula: a(n) = digital_root( P(k,r) ),
 *           where (k,r) are determined by the n‑th term in the
 *           antidiagonal ordering of the array
 *                 A(k,r) = digital_root( ((k-2)·r²-(k-4)·r)/2 )
 *           and k ≥ 3, r ≥ 1.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/min_x.h"

/*  Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined, but harmless */

    /* 1. locate the antidiagonal (sum s) and position j inside it */
    idx_t x    = min_x(n);                /* x = s-3  →  x(x+1)/2 ≥ n */
    idx_t prev = ((unsigned __int128)(x - 1) * x) / 2;   /* cumulative before this diagonal */
    idx_t j    = n - prev;                 /* 1‑based offset inside diagonal (also r) */

    /* 2. compute k and r (the polygonal indices) */
    seq_t k = (seq_t)(x + 3 - j);          /* k = (s-1) - (j-1) = s - j */
    seq_t r = (seq_t)j;                    /* r = j */

    /* 3. value = ((k-2)*r² - (k-4)*r)/2  */
    seq_t r2 = r * r;
    seq_t term = ((seq_t)(k - 2) * r2 - (seq_t)(k - 4) * r) / 2;

    /* 4. digital root: 1 + ((term-1) mod 9), but 9 → 9 */
    seq_t rem = term % 9;
    return rem == (seq_t)0 ? (seq_t)9 : rem;
}
