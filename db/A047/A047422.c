/*
 * A047422  Numbers that are congruent to {1, 2, 3, 4, 5, 6} mod 8.
 * https://oeis.org/A047422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + (n - 6*floor((n-1)/6))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset less than 1 */

    idx_t k = (n - 1) / 6;           /* number of complete 8‑number blocks */
    idx_t r = n - 6 * k;             /* remainder in the current block */
    return (seq_t)(8 * k + r);      /* the n‑th allowed number */
}
