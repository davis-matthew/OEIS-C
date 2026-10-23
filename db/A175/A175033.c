/*
 * A175033  Numbers n such that (ceiling(sqrt(n*n/2)))^2 - n*n/2 = 17/2.
 * https://oeis.org/A175033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175033: a(n) = 6*a(n-2) – a(n-4), with
   a(1)=9, a(2)=15, a(3)=55, a(4)=89.  (Offset 1) */

seq_t a(idx_t n)
{
    /* Handle n < 1 – behaviour below the offset is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Base cases. */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)55;
    if (n == 4) return (seq_t)89;

    /* Sliding window of the last four computed values.
       win[0] = a(k-4), win[1] = a(k-3), win[2] = a(k-2), win[3] = a(k-1) */
    seq_t win[4] = { (seq_t)9, (seq_t)15, (seq_t)55, (seq_t)89 };

    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = (seq_t)6 * win[2] - win[0];  /* a(k) = 6*a(k-2) – a(k-4) */

        /* Advance the window for the next iteration. */
        win[0] = win[1];
        win[1] = win[2];
        win[2] = win[3];
        win[3] = next;
    }

    return win[3];
}
