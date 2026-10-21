/*
 * A092490  a(n) = A058094(n) - 3*A058094(n-1) + A058094(n-2) for n >=4.
 * https://oeis.org/A092490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 9*a(n-3) - 4*a(n-4) - 4*a(n-5) +
 *            a(n-6); Initial terms: a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=5,
 *            a(6)=20, a(7)=75
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the known initial values directly. */
    if (n <= 7) {
        static const seq_t init[8] = {0, 0, 0, 0, 1, 5, 20, 75};
        return init[n];
    }

    /* Sliding window of the last seven terms.
       last[0] = a(k-6), last[1] = a(k-5), ..., last[5] = a(k-1), last[6] = a(k) */
    seq_t last[7];
    last[0] = 0;   /* a(1) */
    last[1] = 0;   /* a(2) */
    last[2] = 0;   /* a(3) */
    last[3] = 1;   /* a(4) */
    last[4] = 5;   /* a(5) */
    last[5] = 20;  /* a(6) */
    last[6] = 75;  /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = 6 * last[6] - 11 * last[5] + 9 * last[4]
                     - 4 * last[3] - 4 * last[2] + last[1];

        /* Shift the window for the next iteration. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = next;
    }

    return last[6];
}
