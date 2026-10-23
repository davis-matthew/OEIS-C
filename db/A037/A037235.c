/*
 * A037235  a(n) = n*(2*n^2 - 3*n + 4)/3.
 * https://oeis.org/A037235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 - 3*n + 4)/3
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=4, a(3)=13, a(n)=4*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4) for n>3 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t to avoid intermediate overflow in intermediate casted operations */
    seq_t nn = (seq_t)n;          /* n */
    seq_t t  = nn * nn;           /* n^2 */

    /* Compute n*(2*n^2 - 3*n + 4) */
    seq_t numerator = nn * (((seq_t)2 * t) - ((seq_t)3 * nn) + (seq_t)4);

    /* Division by 3 is exact for all integer n */
    return numerator / (seq_t)3;
}
