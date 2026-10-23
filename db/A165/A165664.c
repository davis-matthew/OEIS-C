/*
 * A165664  First digit of the decimal expansion of (n^2-2)/7 after the
 *          decimal point.
 * https://oeis.org/A165664
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = floor(10*(n^2-2)/7) mod 10
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic with period 7: a(n) depends only on n mod 7 */
    static const seq_t table[7] = {0, 1, 2, 4, 5, 7, 8};
    idx_t r = n % 7;          /* 0 <= r < 7 */
    idx_t r2 = r * r;         /* r2 <= 36, no overflow */
    idx_t s = (r2 + 5) % 7;   /* (n^2 - 2) mod 7 */
    return table[s];
}
