/*
 * A076767  Triangular numbers with square pyramidal indices.
 * https://oeis.org/A076767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = T(SP(n)) = (n*(n+1)*(2*n+1)/6) *
 *            ((n*(n+1)*(2*n+1)/6)+1)/2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where SP(n) = n*(n+1)*(2*n+1)/6  and T(k) = k*(k+1)/2 */

seq_t a(idx_t n)
{
    /* compute SP(n) = n*(n+1)*(2*n+1)/6   */
    seq_t a1 = (seq_t)n;
    seq_t b1 = (seq_t)n + 1;
    seq_t c1 = 2 * (seq_t)n + 1;

    /* divide by 2 */
    if ((a1 & 1) == 0)      a1 >>= 1;
    else if ((b1 & 1) == 0) b1 >>= 1;
    else                    c1 >>= 1;

    /* divide by 3 */
    if (a1 % 3 == 0)        a1 /= 3;
    else if (b1 % 3 == 0)   b1 /= 3;
    else                    c1 /= 3;

    seq_t sp = a1 * b1 * c1;   /* SP(n) */

    /* compute T(SP(n)) = sp*(sp+1)/2   */
    seq_t sp_plus = sp + 1;

    if ((sp & 1) == 0) sp >>= 1;
    else sp_plus >>= 1;

    seq_t result = sp * sp_plus;
    return result;
}
