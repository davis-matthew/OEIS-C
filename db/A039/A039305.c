/*
 * A039305  Number of distinct quadratic residues mod 8^n.
 * https://oeis.org/A039305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=12, a(3)=87;
   a(n)=8*a(n-1)+a(n-2)-8*a(n-3) for n>3.
   This is equivalent to a(n)=⌊(8^n+10)/6⌋. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;
    if (n == 2U) return (seq_t)12;
    if (n == 3U) return (seq_t)87;

    /* Initial values a(1), a(2), a(3) for the recurrence. */
    seq_t a_nm3 = (seq_t)3;  /* a(n-3) for n=4  -> a(1) */
    seq_t a_nm2 = (seq_t)12; /* a(n-2) for n=4  -> a(2) */
    seq_t a_cur = (seq_t)87; /* a(n-1) for n=4  -> a(3) */

    /* Iterate from n=4 up to the requested index. */
    for (idx_t i = 4U; i <= n; ++i) {
        /* a(n) = 8·a(n-1) + a(n-2) – 8·a(n-3) */
        seq_t next = (a_cur << 3) + a_nm2 - (a_nm3 << 3);

        /* Shift the window of previous three values. */
        a_nm3 = a_nm2;
        a_nm2 = a_cur;
        a_cur = next;
    }
    return a_cur;
}
