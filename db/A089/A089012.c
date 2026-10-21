/*
 * A089012  a(n) = 1 if n is an exponent of the Weyl group W(E_6), 0
 *          otherwise.
 * https://oeis.org/A089012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n ∈ {1,4,5,7,8,11} else 0
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n == 1) || (n == 4) || (n == 5) || (n == 7) || (n == 8) || (n == 11));
}
