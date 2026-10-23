/*
 * A041760  Numerators of continued fraction convergents to sqrt(401).
 * https://oeis.org/A041760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 40*a(n-1) + a(n-2), with a(0) = 20, a(1) = 801
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)20;
    if (n == 1) return (seq_t)801;

    /* Iteratively compute the recurrence up to n */
    seq_t a_nm2 = (seq_t)20;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)801;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a_n = (seq_t)40 * a_nm1 + a_nm2; /* a(n) = 40*a(n-1)+a(n-2) */
        a_nm2 = a_nm1;   /* shift window */
        a_nm1 = a_n;
    }

    return a_nm1;
}
