/*
 * A098526  Expansion of (1+4x^2)/(1-x-16x^5).
 * https://oeis.org/A098526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 16*a(n-5) Initial values: a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=5, a(4)=5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)5;

    /* Keep the last five terms in a circular buffer. */
    seq_t last[5] = {(seq_t)1, (seq_t)1, (seq_t)5, (seq_t)5, (seq_t)5};

    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        /* Compute the next term from the recurrence. */
        next = last[4] + ((seq_t)16 * last[0]);

        /* Shift the buffer: discard the oldest term, make room for next. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = next;
    }

    /* The desired term is now in last[4]. */
    return last[4];
}
