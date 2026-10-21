/*
 * A206950  Number of nonisomorphic graded posets with 0 and non-uniform Hasse
 *          graph of rank n, with no 3-element antichain.
 * https://oeis.org/A206950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 59*a(n-2) + 115*a(n-3) - 109*a(n-4) +
 *            51*a(n-5) - 9*a(n-6), a(0)=0, a(1)=0, a(2)=0, a(3)=3, a(4)=33,
 *            a(5)=259, a(6)=1762
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6‑step recurrence requires only the last six values. */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)0;

    /* Initial values for n=3..6. */
    seq_t a0 = (seq_t)0;  /* a(0)  */
    seq_t a1 = (seq_t)0;  /* a(1)  */
    seq_t a2 = (seq_t)0;  /* a(2)  */
    seq_t a3 = (seq_t)3;  /* a(3)  */
    seq_t a4 = (seq_t)33; /* a(4)  */
    seq_t a5 = (seq_t)259;/* a(5)  */
    seq_t a6 = (seq_t)1762;/* a(6)  */

    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;
    if (n == 6) return a6;

    /* Compute iteratively up to the requested n. */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (seq_t)13 * a6
                     - (seq_t)59 * a5
                     + (seq_t)115 * a4
                     - (seq_t)109 * a3
                     + (seq_t)51 * a2
                     - (seq_t)9   * a1;

        /* Shift the sliding window. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
