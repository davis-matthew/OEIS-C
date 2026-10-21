/*
 * A067417  Triangle with columns built from certain power sequences.
 * https://oeis.org/A067417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is defined by a triangle. For row r and * column m (0 ≤ m ≤
 *            r), * * a(r,m) = 1 if r == m * a(r,m) = (m+3) *
 *            (3*(m+1))^(r-m-1) otherwise * * The 1‑dimensional sequence is
 *            obtained by concatenating the rows: * a(0) = 1; a(1) = 3; a(2) =
 *            1; a(3) = 9; a(4) = 4; a(5) = 1; ... * * The function bel...
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 92, u64 n <= 234,
 *            u128 n <= 670, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* index of first element in row r */
    idx_t m = n - start;             /* column in that row (0 ≤ m ≤ r) */

    if (r == m)                /* diagonal entries are always 1 */
        return (seq_t)1;

    /* compute (m+3) * (3*(m+1))^(r-m-1) */
    seq_t base = (seq_t)(3 * (m + 1));
    idx_t exp  = r - m - 1;

    seq_t pow_val = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow_val *= base;

    return (seq_t)(m + 3) * pow_val;
}
