/*
 * A030102  Base-3 reversal of n (written in base 10).
 * https://oeis.org/A030102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = reverse of n's base‑3 digits interpreted as a base‑3
 *            number
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n > 0)
    {
        seq_t digit = (seq_t)(n % 3);
        res = res * 3 + digit;
        n /= 3;
    }
    return res;
}
