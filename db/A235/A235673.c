/*
 * A235673  Number of (n+1) X (2+1) 0..1 arrays with the difference between
 *          each 2 X 2 subblock maximum and minimum lexicographically
 *          nondecreasing columnwise and nonincreasing rowwise.
 * https://oeis.org/A235673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 9*a(n-2) - 9*a(n-3) + 10*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) Initial values (n = 1 … 6): a(1) = 58, a(2) = 380, a(3)
 *            = 2456, a(4) = 15790, a(5) = 101398, a(6) = 650928 This
 *            implementation evaluates the recurrence for n ≥ 7
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly. */
    if (n <= 6) {
        static const seq_t init[6] = {
            (seq_t)58,
            (seq_t)380,
            (seq_t)2456,
            (seq_t)15790,
            (seq_t)101398,
            (seq_t)650928
        };
        return init[n - 1];
    }

    /* Ring buffer for the last six terms: a(n-1) … a(n-6). */
    static seq_t buf[6];
    buf[0] = (seq_t)58;     /* a(1)   */
    buf[1] = (seq_t)380;    /* a(2)   */
    buf[2] = (seq_t)2456;   /* a(3)   */
    buf[3] = (seq_t)15790;  /* a(4)   */
    buf[4] = (seq_t)101398; /* a(5)   */
    buf[5] = (seq_t)650928; /* a(6)   */

    /* Compute terms from a(7) up to a(n). */
    for (idx_t i = 7; i <= n; ++i) {
        /* Retrieve the required previous values. */
        seq_t a1 = buf[(i - 2) % 6]; /* a(i-1) */
        seq_t a2 = buf[(i - 3) % 6]; /* a(i-2) */
        seq_t a3 = buf[(i - 4) % 6]; /* a(i-3) */
        seq_t a4 = buf[(i - 5) % 6]; /* a(i-4) */
        seq_t a5 = buf[(i - 6) % 6]; /* a(i-5) */
        seq_t a6 = buf[(i - 7) % 6]; /* a(i-6) */

        /* Compute the numerator part of the recurrence. */
        seq_t pos = a1 * (seq_t)8 + a4 * (seq_t)10 + a5 * (seq_t)3;
        seq_t neg = a2 * (seq_t)9 + a3 * (seq_t)9 + a6 * (seq_t)2;

        /* The result is guaranteed to be non‑negative. */
        seq_t val = pos;
        val -= neg;

        /* Store the new term in the buffer, overwriting the old a(i-6). */
        buf[(i - 1) % 6] = val;
    }

    return buf[(n - 1) % 6];
}
