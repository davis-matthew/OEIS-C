/*
 * A037468  a(n)=Sum{d(i)*9^i: i=0,1,...,m}, where Sum{d(i)*5^i: i=0,1,...,m}
 *          is the base 5 representation of n.
 * https://oeis.org/A037468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{i=0}^{m} d_i * 9^i, where d_i are the base‑5 digits of
 *            n
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow9   = 1;
    idx_t temp   = n;
    while (temp > 0) {
        seq_t digit = temp % 5;
        result += digit * pow9;
        pow9 *= 9;
        temp /= 5;
    }
    return result;
}
