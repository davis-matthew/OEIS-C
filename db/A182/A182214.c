/*
 * A182214  Bondage number of the Cartesian product graph G = C_n X K_2.
 * https://oeis.org/A182214
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if n mod 4 = 0 or 1, 3 if n mod 4 = 3, 4 if n mod 4 = 2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)               /* undefined for n < 3, return 0 as a safe default */
        return (seq_t)0;
    idx_t r = n & 3;         /* n modulo 4 */
    switch (r) {
        case 0: return (seq_t)2;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        case 3: return (seq_t)3;
    }
    return (seq_t)0;         /* unreachable but satisfies compiler */
}
