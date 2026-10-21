/*
 * A052624  E.g.f. (1+x^2-2x^3+x^4)/(1-x)^2.
 * https://oeis.org/A052624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)! for n >= 3; with initial terms a(0)=1, a(1)=2,
 *            a(2)=8
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return the fixed small values first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    /* For n >= 3 compute (n+1)! */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n + 1; ++i)
        result *= (seq_t)i; /* cast i to seq_t before multiplication */
    return result;
}
