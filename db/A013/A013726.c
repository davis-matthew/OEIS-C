/*
 * A013726  a(n) = 21^(2*n + 1).
 * https://oeis.org/A013726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 21 * 441^n  (since 441 = 21^2) */
    seq_t result = 21;
    seq_t base   = 441;
    idx_t e = n;

    while (e) {
        if (e & 1) {
            result = result * base;
        }
        base = base * base;
        e >>= 1;
    }

    return result;
}
