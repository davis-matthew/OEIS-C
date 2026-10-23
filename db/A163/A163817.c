/*
 * A163817  Expansion of (1 - x^2) * (1 - x^5) / ((1 - x) * (1 - x^6)) in
 *          powers of x.
 * https://oeis.org/A163817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(0)=1; for n>0, a(n)= 1 if n%6==1, -1 if n%6==5, 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A163817: (1 - x^2)*(1 - x^5)/((1 - x)*(1 - x^6)) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    switch ((n % (idx_t)6)) {
    case 1:
        return (seq_t)1;
    case 5:
        return (seq_t)-1;
    default:
        return (seq_t)0;
    }
}
