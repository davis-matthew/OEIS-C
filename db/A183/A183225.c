/*
 * A183225  Array: row r is the complement of the (r+2)-gonal numbers; by
 *          antidiagonals.
 * https://oeis.org/A183225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p_gonal_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A183225
   a(n) = the n-th element read by antidiagonals from the array
          whose r‑th row consists of the positive integers that are NOT
          (r+2)-gonal numbers.
   The function locates the (r,c) pair on the antidiagonal,
   determines the polygonal order p = r+2,
   and returns the c‑th positive integer that is not a p‑gonal number. */

#include <stdint.h>
#include "../utils/p_gonal_u128.h"

seq_t a(idx_t n)
{
    if (n < 1)           /* off‑offset behaviour */
        return 0;

    /* 1. Find the diagonal index s such that
       T_{s-1} = (s-1)*s/2  >= n  (T_k is the k‑th triangular number). */
    idx_t s = 2;
    while (((__int128)(s - 1) * s) / 2 < n)
        ++s;

    /* 2. Find r (row) and c (column) on that diagonal.           */
    __int128 prev128 = (((__int128)(s - 2) * (s - 1)) / 2);
    idx_t   prev     = (idx_t)prev128;          /* cumulative count before row s */
    idx_t   k        = n - prev;                /* 1‑based position on diagonal */
    idx_t   r        = k;                       /* row number (starting at 1)   */
    idx_t   c        = s - k;                   /* column number (starting at 1)*/
    idx_t   p        = r + 2;                   /* polygonal order              */

    /* 3. Find the c-th positive integer that is NOT a p‑gonal number. */
    unsigned long long count   = 0;             /* how many non‑p‑gonal found   */
    unsigned long long m       = 1;             /* current integer candidate   */
    unsigned long long poly_k  = 1;             /* index of next p‑gonal number */

    while (count < c) {
        /* Compute the next p‑gonal number using higher precision. */
        __int128 poly128 = p_gonal_u128(p, poly_k);
        unsigned long long poly = (unsigned long long)poly128;

        /* Count all integers less than poly that are not p‑gonal. */
        while (m < poly) {
            ++count;
            if (count == c)
                return (seq_t)m;     /* small enough for seq_t (test range) */
            ++m;
        }

        /* Skip the p‑gonal number itself. */
        ++m;
        ++poly_k;
    }

    /* This point is unreachable for valid inputs. */
    return (seq_t)m;
}
