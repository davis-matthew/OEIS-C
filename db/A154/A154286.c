/*
 * A154286  a(n) = E(k)*C(n+k,k) = Euler(k)*binomial(n+k,k) for k=4.
 * https://oeis.org/A154286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*5/24
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 352, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 5;
    result *= ((seq_t)n + 1);
    result *= ((seq_t)n + 2);
    result *= ((seq_t)n + 3);
    result *= ((seq_t)n + 4);
    result /= 24;
    return result;
}
