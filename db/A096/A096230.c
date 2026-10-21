/*
 * A096230  Period 5: repeat [9, 7, 5, 3, 1].
 * https://oeis.org/A096230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 - 2*((n-1) mod 5)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t values[5] = {9, 7, 5, 3, 1};
    /* 0‑based index: (n-1) % 5 */
    idx_t idx = (n - 1) % 5;
    return values[idx];
}
