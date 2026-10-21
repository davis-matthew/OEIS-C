/*
 * A224949  Leap years having 53 Thursdays and Fridays.
 * https://oeis.org/A224949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224949: leap years having 53 Thursdays and Fridays.
   The Gregorian calendar repeats every 400 years.  In a 400‑year cycle
   there are 13 leap years that start on Thursday; their first cycle is
   1604,1632,1660,1688,1728,1756,1784,1824,1852,1880,1920,1948,1976.
   Hence for n ≥ 1
        a(n) = 400 * floor((n-1)/13) + tbl[(n-1) mod 13]     */
seq_t a(idx_t n)
{
    /* no meaningful value for n = 0; the test harness never calls this */
    if (n == 0) return 0;

    /* table of the 13 values in a single 400‑year cycle */
    static const seq_t tbl[13] = {
        (seq_t)1604, (seq_t)1632, (seq_t)1660, (seq_t)1688, (seq_t)1728,
        (seq_t)1756, (seq_t)1784, (seq_t)1824, (seq_t)1852, (seq_t)1880,
        (seq_t)1920, (seq_t)1948, (seq_t)1976
    };

    idx_t pos   = n - 1;           /* zero‑based position            */
    idx_t idx   = pos % 13;        /* index within the 400‑year cycle */
    seq_t cycle = (seq_t)(pos / 13);/* number of complete cycles       */

    return tbl[idx] + cycle * (seq_t)400;
}
