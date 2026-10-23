/*
 * A028098  Expansion of 1/((1-3x)(1-7x)(1-11x)(1-12x)).
 * https://oeis.org/A028098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*12^(n+3)-45*11^(n+3)+18*7^(n+3)-5*3^(n+3))/1440. The
 *            sequence satisfies the 4‑term linear recurrence a(0)=1, a(1)=33,
 *            a(2)=706, a(3)=12462,
 *            a(n)=33*a(n-1)-383*a(n-2)+1803*a(n-3)-2772*a(n-4) for n≥4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices can be returned directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)33;
    if (n == (idx_t)2) return (seq_t)706;
    if (n == (idx_t)3) return (seq_t)12462;

    /* Maintain the last four values. */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)33;         /* a(n-3) */
    seq_t a2 = (seq_t)706;        /* a(n-2) */
    seq_t a3 = (seq_t)12462;      /* a(n-1) */

    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        /* Compute a(n) from the recurrence.
           Use a two‑step addition/subtraction to avoid
           unsigned underflow. */
        seq_t val1 = (seq_t)33 * a3 + (seq_t)1803 * a1;
        seq_t val2 = (seq_t)383 * a2 + (seq_t)2772 * a0;
        seq_t an   = val1 - val2;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
