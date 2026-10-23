/*
 * A026065  a(n) = (d(n)-r(n))/5, where d = A026063 and r is the periodic
 *          sequence with fundamental period (1,4,0,0,0).
 * https://oeis.org/A026065
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) +
 *            3*a(n-7) - a(n-8); Initial values (offset 6): a(6)=14, a(7)=23,
 *            a(8)=36, a(9)=51, a(10)=69, a(11)=90, a(12)=114, a(13)=143; The
 *            sequence is defined for n >= 6
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return any value for indices below the offset. */
    if (n < 6)
        return 0;

    /* For indices 6..13, return the pre‑computed constants. */
    if (n <= 13) {
        static const seq_t base[8] = {14, 23, 36, 51, 69, 90, 114, 143};
        return base[n - 6];
    }

    /* Circular buffer for the most recent 8 terms:
       buf[0] = a(k-8), buf[1] = a(k-7), ..., buf[7] = a(k-1). */
    seq_t buf[8] = {14, 23, 36, 51, 69, 90, 114, 143};

    /* Compute terms up to the requested n = k. */
    for (idx_t k = 14; k <= n; ++k) {
        seq_t new_term;
        new_term = 3 * buf[7];          /* 3*a(k-1)   */
        new_term -= 3 * buf[6];         /* -3*a(k-2)  */
        new_term += buf[5];             /* + a(k-3)   */
        new_term += buf[3];             /* + a(k-5)   */
        new_term -= 3 * buf[2];         /* -3*a(k-6)  */
        new_term += 3 * buf[1];         /* +3*a(k-7)  */
        new_term -= buf[0];             /* - a(k-8)   */

        /* Shift buffer left and insert the new term at the end. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = new_term;
    }

    return buf[7];
}
