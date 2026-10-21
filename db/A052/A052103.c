/*
 * A052103  The third of the three sequences associated with the polynomial
 *          x^3 - 2.
 * https://oeis.org/A052103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u || n == 1u)
        return 0;
    if (n == 2u)
        return 1;

    seq_t a0 = 0; /* a(n-3)   */
    seq_t a1 = 0; /* a(n-2)   */
    seq_t a2 = 1; /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = 3 * a2 - 3 * a1 + 3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
