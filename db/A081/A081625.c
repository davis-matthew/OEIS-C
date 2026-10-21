/*
 * A081625  a(n) = 2*5^n - 3^n.
 * https://oeis.org/A081625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 15*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 7;

    /* Iteratively compute the recurrence for n >= 2. */
    seq_t a0 = 1;    /* a(0) */
    seq_t a1 = 7;    /* a(1) */
    seq_t a;

    for (idx_t i = 2; i <= n; ++i) {
        a  = 8 * a1 - 15 * a0;  /* a(i) = 8*a(i-1) - 15*a(i-2) */
        a0 = a1;                /* shift previous two values */
        a1 = a;
    }

    return a1;  /* a(n) is stored in a1 after the loop */
}
