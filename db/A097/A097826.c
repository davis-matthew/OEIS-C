/*
 * A097826  Partial sums of Chebyshev sequence S(n,11) = U(n,11/2) =
 *          A004190(n).
 * https://oeis.org/A097826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097826: partial sums of Chebyshev S(n,11)=U(n,11/2).
   Recurrence: a(0)=1, a(1)=12,
   a(n) = 11*a(n-1) - a(n-2) + 1   (n ≥ 2)
*/

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)12;
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)12;  /* a(1)   */
    seq_t ai = 0;
    for (idx_t i = 2u; i <= n; ++i) {
        ai = (seq_t)11 * a1 - a0 + (seq_t)1;
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
