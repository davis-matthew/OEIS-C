/*
 * A168455  3^n+n mod 7.
 * https://oeis.org/A168455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n + n) mod 7
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pow3[6] = {1, 3, 2, 6, 4, 5};
    seq_t val = pow3[n % 6];
    val += (seq_t)(n % 7);
    val %= 7;
    return val;
}
