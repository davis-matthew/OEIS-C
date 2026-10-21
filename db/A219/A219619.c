/*
 * A219619  a(n) = n! * (n^4 + n^2 + 1).
 * https://oeis.org/A219619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n^4 + n^2 + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t factorial = 1;
    seq_t sn = n;

    for (idx_t i = 1; i <= n; ++i) {
        factorial *= (seq_t)i;
    }

    seq_t sq = sn * sn;          /* sn^2 */
    seq_t poly = sq * sq + sq + 1; /* n^4 + n^2 + 1 */

    return factorial * poly;
}
