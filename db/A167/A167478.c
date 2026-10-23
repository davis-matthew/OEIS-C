/*
 * A167478  Expansion of (1-2x+6x^2-x^3)/(1-3x+x^2)^2.
 * https://oeis.org/A167478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4); initial values:
 *            a(0)=1, a(1)=4, a(2)=19, a(3)=75
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)75;

    /* Rolling window of the last four values */
    seq_t a0 = (seq_t)1;  /* a(n-4) */
    seq_t a1 = (seq_t)4;  /* a(n-3) */
    seq_t a2 = (seq_t)19; /* a(n-2) */
    seq_t a3 = (seq_t)75; /* a(n-1) */
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = 6*a3 - 11*a2 + 6*a1 - a0 */
        curr  = a3 * 6u;
        curr -= a2 * 11u;
        curr += a1 * 6u;
        curr -= a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return curr;
}
