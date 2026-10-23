/*
 * A171861  Expansion of x*(1+x+x^2) / ( (x-1)*(x^3+x^2-1) ).
 * https://oeis.org/A171861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4), with a(1)=1, a(2)=2, a(3)=4,
 *            a(4)=6 (OEIS offset 1)
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 75, u64 n <= 153,
 *            u128 n <= 311, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)6;

    /* Use a sliding window of the last four values:
       a1 = a(i-1), a2 = a(i-2), a3 = a(i-3), a4 = a(i-4). */
    seq_t a1 = (seq_t)6; /* a(4) */
    seq_t a2 = (seq_t)4; /* a(3) */
    seq_t a3 = (seq_t)2; /* a(2) */
    seq_t a4 = (seq_t)1; /* a(1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = a1 + a2 - a4; /* a(i-1)+a(i-2)-a(i-4) */
        a4 = a3;  /* shift window */
        a3 = a2;
        a2 = a1;
        a1 = val;
    }
    return a1;
}
