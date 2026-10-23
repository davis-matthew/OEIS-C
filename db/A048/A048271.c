/*
 * A048271  a(0) = 1, a(n+1) = -3*a(n) mod 11.
 * https://oeis.org/A048271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n mod 11
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[10] = {
        (seq_t)1, (seq_t)8, (seq_t)9, (seq_t)6,
        (seq_t)4, (seq_t)10, (seq_t)3, (seq_t)2,
        (seq_t)5, (seq_t)7
    };
    return table[n % 10];
}
