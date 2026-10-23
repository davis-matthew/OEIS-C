/*
 * A163305  Numerators of fractions in the approximation of the square root of
 *          5 satisfying: a(n)= (a(n-1)+ c)/(a(n-1)+1); with c=5 and a(1)=0.
 *          Also product of the powers of two and five times the Fibonacci
 *          numbers.
 * https://oeis.org/A163305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), a(1)=0, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;

    /* Iteratively compute the recurrence up to n. */
    seq_t a_nm2 = (seq_t)0; /* a(n-2) */
    seq_t a_nm1 = (seq_t)5; /* a(n-1) */
    seq_t a_n   = (seq_t)0; /* current a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_n   = (a_nm1 * (seq_t)2) + (a_nm2 * (seq_t)4);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
