/*
 * A199399  Binary XOR of 5^k as k varies from 0 to n.
 * https://oeis.org/A199399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = XOR_{k=0}^{n} 5^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;       /* 5^0 */
    for (idx_t i = 0; i <= n; ++i) {
        result ^= power;
        power *= (seq_t)5;
    }
    return result;
}
