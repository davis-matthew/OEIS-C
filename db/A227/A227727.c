/*
 * A227727  a(n) is the least r>=3 such that the difference between n and the
 *          nearest r-gonal number<=n is an r-gonal number.
 * https://oeis.org/A227727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 433, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   poly_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the OEIS definition of A227727:
   a(n) is the smallest integer r ≥ 3 such that the difference
   between n and the largest r‑gonal number ≤ n is itself an r‑gonal
   number.  This code works for any build: seq_t may be an unsigned
   integer type or an arbitrary‑precision integer class.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/poly_value.h"

/* Integer square root for unsigned __int128 values, result fits in idx_t */
static inline idx_t isqrt128(unsigned __int128 n)
{
    idx_t x = 0;
    /* start with the highest set bit that fits in idx_t (64‑bit) */
    idx_t bit = (idx_t)1ULL << 63;
    while ((unsigned __int128)bit > n) bit >>= 1;
    while (bit) {
        unsigned __int128 nx = (unsigned __int128)(x + bit) * (x + bit);
        if (nx <= n) x += bit;
        bit >>= 1;
    }
    return x;
}

/* Return the index x of the greatest r‑gonal number P_r(x) not exceeding n.
   r and n are idx_t; x is also idx_t. */
static inline idx_t poly_index(idx_t r, idx_t n)
{
    int64_t rs = (int64_t)r;
    int64_t a  = rs - 4;          /* may be negative only for r = 3 */
    int64_t b  = rs - 2;          /* always ≥1 for r ≥ 3 */

    /* Compute the discriminant: (r-4)² + 8*n*(r-2) */
    __int128 discr = (__int128)a * a + (__int128)8 * n * b;
    unsigned __int128 sum = (unsigned __int128)discr;

    idx_t sqrt_sum = isqrt128(sum);

    __int128 num    = (__int128)a + (__int128)sqrt_sum;
    __int128 denom  = (__int128)2 * b;        /* >0 for r ≥ 3 */

    return (idx_t)(num / denom);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    for (idx_t r = 3; ; ++r) {
        /* Largest r‑gonal number not exceeding n */
        idx_t x = poly_index(r, n);
        seq_t p = poly_value(r, x);

        /* difference between n and that r‑gonal number */
        idx_t p_idx = (idx_t)p;          /* p ≤ n → fits in idx_t */
        idx_t diff = n - p_idx;          /* diff ≤ n */

        /* diff itself is an r‑gonal number? */
        if (diff == 0) return (seq_t)r;

        idx_t y = poly_index(r, diff);
        seq_t p2 = poly_value(r, y);
        if ((seq_t)diff == p2) return (seq_t)r;
    }
}
