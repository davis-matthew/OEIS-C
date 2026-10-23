/*
 * A100451  a(n) = 0 for n <= 2; for n >= 3, a(n) =
 *          (n-2)*floor((n^2-2)/(n-2)).
 * https://oeis.org/A100451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n+2) for n >= 5; a(1) = 0, a(2) = 0, a(3) = 7,
 *            a(4) = 14
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return 0;
    }
    if (n == 3) {
        return 7;
    }
    if (n == 4) {
        return 14;
    }
    seq_t m = (seq_t)n;          /* cast index to seq_t */
    return (m - 2) * (m + 2);
}
