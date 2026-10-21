/*
 * A232162  Number of Weyl group elements, not containing an s_r factor, which
 *          contribute nonzero terms to Kostant's weight multiplicity formula
 *          when computing the multiplicity of the zero-weight in the adjoint
 *          representation for the Lie algebra of type B and rank n.
 * https://oeis.org/A232162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) with initial values
 *            a(0)=0, a(1)=0, a(2)=2, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 3;

    seq_t a0 = 0;   /* a(n-4) */
    seq_t a1 = 0;   /* a(n-3) */
    seq_t a2 = 2;   /* a(n-2) */
    seq_t a3 = 3;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = a3 + a2 + 3 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
