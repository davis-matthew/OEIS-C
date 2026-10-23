/*
 * A189743  a(1)=4, a(2)=4, a(n) = 4*a(n-1) + 4*a(n-2).
 * https://oeis.org/A189743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 4, a(2) = 4, a(n) = 4*a(n-1) + 4*a(n-2) */
seq_t a(idx_t n)
{
    /* Below the offset the value is irrelevant – return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Base cases: a(1) = a(2) = 4. */
    if (n <= 2)
        return (seq_t)4;

    /* Compute the sequence iteratively from the start up to n. */
    seq_t a_nm2 = (seq_t)4;   /* a(1) */
    seq_t a_nm1 = (seq_t)4;   /* a(2) */
    seq_t a_n   = a_nm1;      /* placeholder */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        a_n   = (seq_t)4 * a_nm1 + (seq_t)4 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
