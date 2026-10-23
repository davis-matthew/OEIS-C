/*
 * A124280  Expansion of 1/(1-x-x^2+x^3-x^4).
 * https://oeis.org/A124280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4), a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=2
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;
    if (n == 3u) return (seq_t)2;

    /* Sliding window for the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)2;   /* a(n-2) */
    seq_t a3 = (seq_t)2;   /* a(n-1) */
    seq_t an = 0;          /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + a2 - a1 + a0;  /* a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
