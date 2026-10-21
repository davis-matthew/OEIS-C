/*
 * A071934  a(n) = Sum_{i=1..n} K(i+1,i), where K(x,y) is the Kronecker symbol
 *          (x/y).
 * https://oeis.org/A071934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - 2*⌈n/8⌉ + (2 if n ≡ 1 (mod 8) else 0)
 * Range:     u8 n <= 341, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t ceil_div = (n + 7) / 8;          /* integer ceil(n/8) */
    seq_t result = n_seq - (seq_t)2 * ceil_div;
    if ((n % 8) == 1)
        result += (seq_t)2;
    return result;
}
