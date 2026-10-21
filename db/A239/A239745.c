/*
 * A239745  a(n) = (3*2^(n+2) + n*(n+5))/2 - 6.
 * https://oeis.org/A239745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239745: a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4)
   with a(0)=0, a(1)=9, a(2)=25, a(3)=54 */

seq_t a(idx_t n)
{
    /* Handle the first four indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)54;

    /* Recurrence: keep only the last four values */
    seq_t a_nm4 = (seq_t)0;   /* a(n-4) */
    seq_t a_nm3 = (seq_t)9;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)25;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)54;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)5 * a_nm1
                   - (seq_t)9 * a_nm2
                   + (seq_t)7 * a_nm3
                   - (seq_t)2 * a_nm4;

        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
