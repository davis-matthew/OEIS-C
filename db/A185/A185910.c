/*
 * A185910  Array: T(n,k) = n^2 + k - 1, by antidiagonals.
 * https://oeis.org/A185910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n^2 + k – 1 where (n,k) is the element of the array
 *            T(n,k) = n^2 + k – 1 visited by antidiagonals. For a given term
 *            index m (m ≥ 1) let S be the smallest integer such that
 *            (S‑1)*S/2 ≥ m. Then the element is at row r and column c with r
 *            = m – (S‑2)*(S‑1)/2, c = S – r, and a(m) = r^2 + c – 1
 * Range:     u8 n <= 135, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* protection for offset < 1 – undefined */
        return 0;

    const uint64_t m = n;          /* term index (1‑based)          */

    /* ----- Find the diagonal sum S ----- */
    long double d = sqrtl((long double)1.0 + (long double)8.0 * (long double)m);
    uint64_t S = (uint64_t)((1.0 + d) / 2.0);

    /* Adjust S to satisfy (S‑1)*S/2 < m ≤ S*(S‑1)/2 */
    while ((unsigned __int128)(S - 1) * S / 2 >= m)
        S--;
    while ((unsigned __int128)S * (S - 1) / 2 < m)
        S++;

    /* ----- Compute offset within the diagonal ----- */
    unsigned __int128 prev = (unsigned __int128)(S - 1) * (S - 2) / 2;
    uint64_t offset = m - (uint64_t)prev - 1;   /* 0‑based offset */
    uint64_t row = offset + 1;
    uint64_t col = S - row;

    /* ----- Compute the value ----- */
    seq_t row_t = (seq_t)row;
    seq_t col_t = (seq_t)col;
    seq_t value = row_t * row_t + col_t - 1;
    return value;
}
