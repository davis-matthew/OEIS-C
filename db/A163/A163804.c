/*
 * A163804  Expansion of (1 - x) * (1 - x^4) / ((1 - x^2) * (1 - x^3)) in
 *          powers of x.
 * https://oeis.org/A163804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(3k)=0 (k>0), a(3k+1)=-1, a(3k+2)=1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    switch (n % 3ULL)
    {
        case 1: return (seq_t)-1;
        case 2: return (seq_t)1;
        default: return (seq_t)0; /* n%3==0 and n>0 */
    }
}
