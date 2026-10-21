/*
 * A076014  Triangle in which m-th entry of n-th row is m^(n-1).
 * https://oeis.org/A076014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = m^(n-1) where m is the column in row n of the triangle *
 *            T(r, c) = c^(r-1) for 1 <= c <= r * * The sequence a(n) is the
 *            triangle read row by row: 1; 1,2; 1,4,9; … * * To compute a(n):
 *            * 1. Find row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n. * 2.
 *            Compute column c = n - (r-1)*r/2. * 3. Re...
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for 64‑bit values (non‑negative return) */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n / 2 + 1;
    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* compute base^exp in seq_t; exp is small (≤ row‑1, ≤ ~140 for n≤10000) */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* under‑offset values are irrelevant */
        return 0;

    /* find the row r (1‑based) containing the n‑th element */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* position within the row */
    idx_t prev_items = (r - 1) * r / 2;
    idx_t c = n - prev_items;          /* 1‑based column */
    idx_t exp = r - 1;                 /* exponent */

    return pow_seq_t((seq_t)c, exp);
}
