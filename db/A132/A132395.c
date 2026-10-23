/*
 * A132395  a(n) = (11^(n+2) + 12^(2*n+1))/133.
 * https://oeis.org/A132395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (11^(n+2) + 12^(2*n+1))/133 Recurrence: a(0)=1, a(1)=23,
 *            a(n+2) = 155*a(n+1) - 1584*a(n) for n>=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)23;

    /* Compute the sequence iteratively using the recurrence
       a(n+2) = 155*a(n+1) - 1584*a(n) */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)23;  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (155 * a_nm1) - (1584 * a_nm2);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
