/*
 * A233255  Number of n X 7 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabeled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5504*a(n-1) - 6438912*a(n-2) + 2774532096*a(n-3) -
 *            478351982592*a(n-4) + 27831388078080*a(n-5) Recurrence initial
 *            values: a(1) = 528 a(2) = 1992704 a(3) = 8050180096 a(4) =
 *            32835998056448 a(5) = 134179300927602688
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage */
seq_t a(idx_t n)
{
    /* Seed values for n = 1..5 */
    static const seq_t seeds[5] = {
        528ULL,
        1992704ULL,
        8050180096ULL,
        32835998056448ULL,
        134179300927602688ULL
    };

    /* n is unsigned; avoid underflow if n < 1 */
    if (n <= 1) {
        return (n == 0) ? (seq_t)0 : seeds[0];
    }
    if (n <= 5) return seeds[n - 1];

    /* Buffer holding the last five terms:
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[4] = a(k-5) */
    seq_t buf[5];
    buf[0] = seeds[4]; /* a(5) */
    buf[1] = seeds[3];
    buf[2] = seeds[2];
    buf[3] = seeds[1];
    buf[4] = seeds[0];

    /* Compute a(k) for k = 6 .. n */
    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = buf[0] * 5504ULL
                   - buf[1] * 6438912ULL
                   + buf[2] * 2774532096ULL
                   - buf[3] * 478351982592ULL
                   + buf[4] * 27831388078080ULL;

        /* Shift the buffer: drop the oldest term */
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0];
}
