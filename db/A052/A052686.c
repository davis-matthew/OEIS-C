/*
 * A052686  Expansion of e.g.f. x^2*(1+3*x-3*x^2)/(1-x).
 * https://oeis.org/A052686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n<2, =2 for n=2, =24 for n=3 or 4, and =n! for n>=5
 *            (recurrence a(n)=n*a(n-1) with a(4)=24)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 2;
    if (n == 3 || n == 4) return 24;

    seq_t fact = 24;          /* fact = 4! */
    for (idx_t i = 5; i <= n; ++i)
        fact *= (seq_t)i;
    return fact;
}
