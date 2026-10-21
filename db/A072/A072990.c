/*
 * A072990  a(2) = 1000, a(n) = 18 - n for 3 <= n <= 9, a(n) = 8 for n >= 10.
 * https://oeis.org/A072990
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(2) = 1000, a(n) = 18 - n for 3 ≤ n ≤ 9, a(n) = 8 for n ≥ 10 (offset 2) */
seq_t a(idx_t n)
{
    if (n == 2) {
        return (seq_t)1000;
    }
    if (n >= 3 && n <= 9) {
        return (seq_t)(18 - (seq_t)n);
    }
    return (seq_t)8;
}
