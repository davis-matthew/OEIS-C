/*
 * A199762  a(n) = (9*11^n+1)/2.
 * https://oeis.org/A199762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*11^n+1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power = 1;           /* holds 11^n */
    seq_t base   = 11;         /* base 11 */
    idx_t exp = n;

    while (exp) {
        if (exp & 1)
            power = power * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }

    seq_t val = (power * 9 + 1) / 2;
    return val;
}
