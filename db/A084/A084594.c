/*
 * A084594  a(n) = Sum_{r=0..2^(n-1)} Binomial(2^n,2r)*3^r.
 * https://oeis.org/A084594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((1+sqrt(3))^(2^n)+(1-sqrt(3))^(2^n))/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n >= 1,
   a(n) = 2*a(n-1)^2 - (-1)^{2^{n-1}} * 2^{2^{n-1}} .
   For n = 1 the sign is +, for n >= 2 the sign is -.  */

seq_t a(idx_t n)
{
    /* a(0) */
    seq_t result = 1;
    if (n == 0) return result;

    /* g_1 = 2^(2^{0}) = 2^1 = 2 */
    seq_t pow = 2;

    /* First step (n = 1) with positive sign */
    result = 2 * result * result + pow;   /* a(1) = 2*a0^2 + 2 */
    pow = pow * pow;                     /* prepare 2^2 = 4 for n = 2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* For n >= 2 the sign is negative */
        result = 2 * result * result - pow;
        pow = pow * pow; /* next pow = 2^(2^{i}) = (previous pow)^2 */
    }

    return result;
}
