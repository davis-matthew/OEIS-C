/*
 * A037670  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0.
 * https://oeis.org/A037670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4) (n ≥ 5) Initial values:
 *            a(1)=3, a(2)=23, a(3)=161, a(4)=1130
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four initial terms directly. */
    if (n == 1) return 3;
    if (n == 2) return 23;
    if (n == 3) return 161;
    if (n == 4) return 1130;

    /* Circular buffer holding the last four computed terms:
       am4 = a(n-4), am3 = a(n-3), am2 = a(n-2), am1 = a(n-1).
       Initially they correspond to the four known terms. */
    seq_t am4 = 3;
    seq_t am3 = 23;
    seq_t am2 = 161;
    seq_t am1 = 1130;

    /* Compute subsequent terms up to the requested index. */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t an = 7 * am1 + am3 - 7 * am4;
        am4 = am3;
        am3 = am2;
        am2 = am1;
        am1 = an;
    }

    return am1;
}
