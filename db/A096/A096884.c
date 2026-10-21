/*
 * A096884  a(n) = 101^n.
 * https://oeis.org/A096884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 101^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = (seq_t)101;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
