/*
 * A170783  a(n) = n^9*(n + 1)/2.
 * https://oeis.org/A170783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n + 1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 7643,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res;
    /* Divide by 2 early to prevent overflow on 64‑bit builds */
    if ((nn & 1) == 0) {
        /* n is even: (n/2) * n^8 * (n+1) */
        res = nn / 2;
        for (idx_t i = 0; i < 8; ++i) res *= nn;   /* n^8 */
        res *= (nn + 1);
    } else {
        /* n is odd: ((n+1)/2) * n^9 */
        res = (nn + 1) / 2;
        for (idx_t i = 0; i < 9; ++i) res *= nn;   /* n^9 */
    }
    return res;
}
