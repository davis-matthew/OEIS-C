/*
 * A196804  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,2,3,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3) + 17*a(n-4) + 3*a(n-5) -
 *            4*a(n-6) (empirical recurrence), with initial values a(1)=1,
 *            a(2)=3, a(3)=16, a(4)=37, a(5)=120, a(6)=420
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* If n is less than 1, the result is irrelevant; return 0 by safety. */
    if (n < 1) return (seq_t)0;

    /* Base values for n = 1..6 */
    static const seq_t base[6] = {
        (seq_t)1,   /* a(1) */
        (seq_t)3,   /* a(2) */
        (seq_t)16,  /* a(3) */
        (seq_t)37,  /* a(4) */
        (seq_t)120, /* a(5) */
        (seq_t)420  /* a(6) */
    };

    if (n <= 6) {
        /* Direct return for the first six terms. */
        return base[n - 1];
    }

    /* We'll keep the last six terms to compute the recurrence. */
    seq_t seq[6];
    /* seq[0] = a(n-1), seq[1] = a(n-2), ..., seq[5] = a(n-6) */
    seq[0] = base[5]; /* a(6) */
    seq[1] = base[4]; /* a(5) */
    seq[2] = base[3]; /* a(4) */
    seq[3] = base[2]; /* a(3) */
    seq[4] = base[1]; /* a(2) */
    seq[5] = base[0]; /* a(1) */

    idx_t i;
    for (i = 7; i <= n; ++i) {
        seq_t next = 2 * seq[0] + 2 * seq[1] + 3 * seq[2] + 17 * seq[3] + 3 * seq[4] - 4 * seq[5];
        /* Shift the buffer: drop the oldest term and insert 'next' at the front. */
        seq[5] = seq[4];
        seq[4] = seq[3];
        seq[3] = seq[2];
        seq[2] = seq[1];
        seq[1] = seq[0];
        seq[0] = next;
    }

    return seq[0];
}
