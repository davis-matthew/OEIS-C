/*
 * A218009  Binomial transform of A212831(n).
 * https://oeis.org/A218009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 16*a(n-3) - 8*a(n-4) for n ≥ 6
 *            Initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=12, a(4)=30,
 *            a(5)=70 This function works for any order of calls and any
 *            idx_t/seq_t compilation target (C11 or C++17)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first six terms directly */
    if (n == 0U) return (seq_t)0;
    if (n == 1U) return (seq_t)1;
    if (n == 2U) return (seq_t)4;
    if (n == 3U) return (seq_t)12;
    if (n == 4U) return (seq_t)30;
    if (n == 5U) return (seq_t)70;

    /* For n ≥ 6 we use the recurrence.
       We keep the last four terms in a sliding window. */
    seq_t a0 = (seq_t)4;   /* a(2) */
    seq_t a1 = (seq_t)12;  /* a(3) */
    seq_t a2 = (seq_t)30;  /* a(4) */
    seq_t a3 = (seq_t)70;  /* a(5) */
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 14*a(i-2) + 16*a(i-3) - 8*a(i-4) */
        next = ((seq_t)6 * a3) - ((seq_t)14 * a2) + ((seq_t)16 * a1) - ((seq_t)8 * a0);
        /* Slide the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
