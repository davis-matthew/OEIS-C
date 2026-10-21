/*
 * A052714  a(n) = 2^(n-1) * n! * Catalan(n-1) for n > 0 with a(0) = 0.
 * https://oeis.org/A052714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * 4*(2n-3) for n ≥ 2; a(1)=1, a(0)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 1;
    for (idx_t k = 1; k < n; ++k)
        result *= (seq_t)(4 * (2 * k - 1));
    return result;
}
