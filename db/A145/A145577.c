/*
 * A145577  Period 9: repeat [0, 5, 7, 6, 2, 4, 3, 8, 1].
 * https://oeis.org/A145577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145577: period‑9 repetition [0,5,7,6,2,4,3,8,1]
   a(n) = a(n-9) for n ≥ 9.
   Implementation uses the period directly. */
seq_t a(idx_t n)
{
    static const seq_t period[9] = {0, 5, 7, 6, 2, 4, 3, 8, 1};
    return period[n % 9];
}
