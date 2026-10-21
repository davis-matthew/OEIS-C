/*
 * A052922  Expansion of 1/(1 - 2*x^3 - x^4).
 * https://oeis.org/A052922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-3) + a(n-4) with initial values a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=2 (OEIS A052922)
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 70, u64 n <= 136,
 *            u128 n <= 269, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Baseline values for n < 4 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)2;

    /* Use a 4‑element rolling buffer to compute the recurrence
       without allocating large arrays. */
    seq_t buf[4];
    buf[0] = (seq_t)1;  /* a(0) */
    buf[1] = (seq_t)0;  /* a(1) */
    buf[2] = (seq_t)0;  /* a(2) */
    buf[3] = (seq_t)2;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* indices modulo 4 */
        idx_t i3 = (i + 1) % 4; /* (i-3) mod 4 */
        idx_t i4 = (i + 0) % 4; /* (i-4) mod 4 */
        buf[i % 4] = (seq_t)2 * buf[i3] + buf[i4];
    }
    return buf[n % 4];
}
