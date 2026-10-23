/*
 * A199414  a(n) = 10*6^n+1.
 * https://oeis.org/A199414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*6^n+1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = (seq_t)1;
    seq_t base = (seq_t)6;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1u)          /* multiply by current power when bit is set */
            pow = pow * base;
        base = base * base;    /* square the base for next bit */
        exp >>= 1;             /* move to next bit */
    }
    return pow * (seq_t)10 + (seq_t)1;
}
