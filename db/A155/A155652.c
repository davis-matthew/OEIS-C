/*
 * A155652  a(n) = 9^n - 6^n + 1.
 * https://oeis.org/A155652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 6^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow9 = 1;
    seq_t pow6 = 1;
    idx_t i;

    for (i = 0; i < n; ++i) {
        pow9 *= (seq_t)9;
        pow6 *= (seq_t)6;
    }

    seq_t res = pow9;
    res -= pow6;
    res += (seq_t)1;
    return res;
}
