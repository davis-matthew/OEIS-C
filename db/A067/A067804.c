/*
 * A067804  Triangle read by rows: T(n,k) is the number of walks (each step
 *          +-1) of length 2n which have a cumulative value of 0 last at step
 *          2k.
 * https://oeis.org/A067804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 170, u64 n <= 594,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r,k) where r is the row containing the n‑th element
 * (0‑based), k = n - r(r+1)/2, and
 *   T(r,k) = C(2k,k) * C(2(r-k), r-k)
 *
 * The function computes the binomial coefficient multiplicatively using
 * 128‑bit intermediates to avoid overflow on 64‑bit builds.  It works for
 * any seq_t (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128
 * or an arbitrary‑precision integer type), because the division at each
 * step guarantees the intermediate stays in range.
 */

#include <stdint.h>   /* For idx_t and seq_t types */

/* Compute binomial coefficient C(n,k) as seq_t.
 * Works for all seq_t sizes that support assignment from an unsigned
 * long long (including standard unsigned integer types and many
 * arbitrary‑precision types). */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */

    seq_t res = 1;                     /* initialise result */

    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;          /* numerator term */
        idx_t den = i;                  /* denominator term */

        /* Multiply with 128‑bit intermediate to avoid 64‑bit overflow. */
        __int128 temp = (__int128)res * num;
        temp /= den;

        res = (seq_t)temp;
    }

    return res;
}

/* A067804 term: flatten the triangle T(n,k) row by row. */
seq_t a(idx_t n)
{
    /* Determine row r such that n lies in this row.
     * Number of terms up to row r is (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t cum = 1;          /* terms up to row 0 */

    while (cum <= n) {
        ++r;
        cum += r + 1;       /* add the (r+1) terms of the next row */
    }

    idx_t start = cum - (r + 1);   /* first index of this row   */
    idx_t k = n - start;           /* position within the row   */

    /* Compute the two binomial factors in the formula. */
    seq_t t1 = binom_idx(2 * k, k);
    seq_t t2 = binom_idx(2 * (r - k), r - k);

    return t1 * t2;           /* product fits seq_t for all valid inputs */
}
