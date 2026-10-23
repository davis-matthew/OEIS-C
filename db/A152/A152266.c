/*
 * A152266  a(n) = ((9 + sqrt(7))^n + (9 - sqrt(7))^n)/2.
 * https://oeis.org/A152266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9 + sqrt(7))^n + (9 - sqrt(7))^n)/2 Recurrence: a(0) =
 *            1, a(1) = 9, a(n) = 18*a(n-1) - 74*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 9;

    seq_t a0 = 1;     /* a(0) */
    seq_t a1 = 9;     /* a(1) */
    seq_t an = 0;     /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * 18 - a0 * 74;
        a0 = a1;
        a1 = an;
    }
    return an;
}
