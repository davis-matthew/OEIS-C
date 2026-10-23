/*
 * A133594  a(n) = 3*a(n-1)+12*a(n-2) for n>=3, a(0)=1, a(1)=3, a(2)=18 .
 * https://oeis.org/A133594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 12*a(n-2) Initial values: a(0) = 1, a(1) = 3,
 *            a(2) = 18. Computes the n-th term of OEIS A133594
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)18;

    /* Use the recurrence iteratively; only keep the last three values. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)18;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = 3 * a2 + 12 * a1;
        a0 = a1;  /* shift for next iteration */
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
