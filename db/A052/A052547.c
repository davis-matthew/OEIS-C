/*
 * A052547  Expansion of (1-x)/(1-x-2*x^2+x^3).
 * https://oeis.org/A052547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3), with a(0) = 1, a(1) = 0, a(2)
 *            = 2
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three seed values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* Use three local variables to keep the last three terms */
    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)0;  /* a(n-2) */
    seq_t a2 = (seq_t)2;  /* a(n-1) */

    /* Iterate from n=3 up to the requested n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a2 + (seq_t)2 * a1 - a0; /* a(i) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
