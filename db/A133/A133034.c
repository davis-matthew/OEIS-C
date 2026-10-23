/*
 * A133034  First differences of Padovan sequence A000931.
 * https://oeis.org/A133034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) + a(n-3) a(0) = -1, a(1) = 0, a(2) = 1
 * Range:     i8 n <= 27, i16 n <= 47, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small initial values */
    if (n == (idx_t)0) return (seq_t)-1;
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)1;

    /* Circular buffer of the last three values */
    seq_t buf[3];
    buf[0] = (seq_t)-1; /* a(0) */
    buf[1] = (seq_t)0;  /* a(1) */
    buf[2] = (seq_t)1;  /* a(2) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        idx_t idx1 = (i - (idx_t)2) % (idx_t)3; /* a(i-2) */
        idx_t idx2 = (i - (idx_t)3) % (idx_t)3; /* a(i-3) */
        seq_t next = buf[idx1] + buf[idx2];
        buf[i % (idx_t)3] = next;
    }

    return buf[n % (idx_t)3];
}
