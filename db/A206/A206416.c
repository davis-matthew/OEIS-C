/*
 * A206416  Achromatic number of K_4 X K_n.
 * https://oeis.org/A206416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor(5*n/3) for n ≥ 25; special values for 1 ≤ n ≤ 24
 *            (achromatic numbers of K4 × Kn)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first 24 terms explicitly */
    if (n >= 1 && n <= 24) {
        static const seq_t tbl[24] = {
            4, 5, 6, 8, 10, 12, 14, 16, 18, 20,
            22, 24, 24, 26, 28, 29, 30, 32, 33, 34,
            36, 37, 38, 40
        };
        return tbl[n - 1];
    }
    /* For n ≥ 25 use the simple formula (integer division is floor) */
    return (seq_t)(5 * (seq_t)n / 3);
}
