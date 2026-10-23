/*
 * A142990  a(1) = 1, a(2) = 7, a(n+2) = 7*a(n+1)+(n+1)*(n+3)*a(n).
 * https://oeis.org/A142990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142990
   Recurrence:  a(1) = 1, a(2) = 7,
                 a(n+2) = 7*a(n+1) + (n+1)*(n+3)*a(n)
   Indexing starts at 1 (a(1) is the first term). */

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)7;

    /* iterate using two previous values only */
    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)7; /* a(n-1) */
    seq_t a_n   = 0;        /* a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = a_nm1 * (seq_t)7;

        seq_t factor = ((seq_t)(i - 1)) * ((seq_t)(i + 1));
        a_n += a_nm2 * factor;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
