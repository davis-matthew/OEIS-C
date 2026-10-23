/*
 * A027116  a(n) = A027113(n, n+3).
 * https://oeis.org/A027116
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A027113(n,n+3). * The sequence is defined by the
 *            recurrence (valid for n>7): * a(n) = 3*a(n-1) - 2*a(n-2) -
 *            a(n-4) + a(n-5) * with initial values * a(3) = 7, a(4) = 23,
 *            a(5) = 56, a(6) = 120, a(7) = 241 * * This implementation
 *            evaluates the recurrence iteratively from n=3 * up to the
 *            reque...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset, the behaviour is irrelevant */
    if (n < 3)
        return 0;

    /* Return the first few terms directly */
    if (n == 3) return 7;
    if (n == 4) return 23;
    if (n == 5) return 56;
    if (n == 6) return 120;
    if (n == 7) return 241;

    /* Temporary variables holding the last five terms:
     * t3 = a(k-5), t4 = a(k-4), t5 = a(k-3), t6 = a(k-2), t7 = a(k-1) */
    seq_t t3 = 7;   /* a(3)   */
    seq_t t4 = 23;  /* a(4)   */
    seq_t t5 = 56;  /* a(5)   */
    seq_t t6 = 120; /* a(6)   */
    seq_t t7 = 241; /* a(7)   */

    for (idx_t k = 8; k <= n; ++k) {
        seq_t new_val = 3 * t7 + t3; /* 3*a(k-1) + a(k-5) */
        new_val -= 2 * t6;           /* subtract 2*a(k-2) */
        new_val -= t4;               /* subtract a(k-4)   */

        /* Shift the window for the next iteration */
        t3 = t4;
        t4 = t5;
        t5 = t6;
        t6 = t7;
        t7 = new_val;
    }

    return t7; /* a(n) */
}
