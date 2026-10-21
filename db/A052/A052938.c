/*
 * A052938  Expansion of (1 + 2*x - 2*x^2)/( (1+x)*(1-x)^2 ).
 * https://oeis.org/A052938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 506, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052938: a(0)=1, a(n) = -a(n-1) + n + 3
   Equivalent closed form:
     a(n) = (n/2 + 1)         if n is even
     a(n) = ((n-1)/2 + 3)     if n is odd
   (which is the same as (3*(-1)^(n+1) + 2*n + 7)/4) */
seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)(n / 2 + 1);
    else
        return (seq_t)((n - 1) / 2 + 3);
}
