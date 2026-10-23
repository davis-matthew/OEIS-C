/*
 * A192473  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=1+x^n+x^(2n+2).
 * https://oeis.org/A192473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4) for n>4 * Initial
 *            values: a(1)=4, a(2)=9, a(3)=23, a(4)=58
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0)          /* not used for valid input, but safe */
        return (seq_t)0;

    /* Handle the first four terms explicitly */
    if (n == 1)  return (seq_t)4;
    if (n == 2)  return (seq_t)9;
    if (n == 3)  return (seq_t)23;
    if (n == 4)  return (seq_t)58;

    /* Iteratively apply the recurrence using only four temporaries */
    seq_t a1 = (seq_t)4;    /* a(n-4)  */
    seq_t a2 = (seq_t)9;    /* a(n-3)  */
    seq_t a3 = (seq_t)23;   /* a(n-2)  */
    seq_t a4 = (seq_t)58;   /* a(n-1)  */

    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = 4 * a4 - 3 * a3 - 2 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
