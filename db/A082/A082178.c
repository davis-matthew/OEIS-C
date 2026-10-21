/*
 * A082178  Professor Umbugio's sequence A082176 divided by 2*7*53*139 =
 *          103138.
 * https://oeis.org/A082178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7266*a(n-1) - 19690571*a(n-2) + 23585007306*a(n-3) * -
 *            10533473613720*a(n-4) for n > 3; * a(0)=0, a(1)=0, a(2)=2,
 *            a(3)=10899
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)10899;

    /* Sliding window for the last four terms */
    seq_t t0 = (seq_t)0;          /* a(n-4) */
    seq_t t1 = (seq_t)0;          /* a(n-3) */
    seq_t t2 = (seq_t)2;          /* a(n-2) */
    seq_t t3 = (seq_t)10899;      /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)7266 * t3;
        next -= (seq_t)19690571 * t2;
        next += (seq_t)23585007306ULL * t1;
        next -= (seq_t)10533473613720ULL * t0;

        t0 = t1;  t1 = t2;  t2 = t3;  t3 = next;
    }
    return t3;
}
