/*
 * A212343  a(n) = (n+1)*(n-2)*(n-3)/2.
 * https://oeis.org/A212343
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n-2)*(n-3)/2 Initial values: a(2)=0, a(3)=0,
 *            a(4)=5, a(5)=18,
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A212343 for any n >= 2.
   It returns 0 for n <= 3, otherwise evaluates
   (n+1)*(n-2)*(n-3)/2 using only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n <= 3u) return 0;
    seq_t tn = (seq_t)n;
    /* (n-2)*(n-3) is always even for n >= 4, so divide by 2 first */
    seq_t temp = (tn - 2u) * (tn - 3u);
    temp /= 2u;
    return (tn + 1u) * temp;
}
