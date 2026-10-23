/*
 * A028023  Expansion of 1/((1-2x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A028023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - 403*a(n-2) + 1866*a(n-3) - 2376*a(n-4)
 *            (n>=4), with initial values a(0)=1, a(1)=34, a(2)=753,
 *            a(3)=13766
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)753;
    if (n == 3) return (seq_t)13766;

    seq_t a0 = (seq_t)1;    /* a(0)   */
    seq_t a1 = (seq_t)34;   /* a(1)   */
    seq_t a2 = (seq_t)753;  /* a(2)   */
    seq_t a3 = (seq_t)13766;/* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t ai = ((seq_t)34 * a3)
                 - ((seq_t)403 * a2)
                 + ((seq_t)1866 * a1)
                 - ((seq_t)2376 * a0);

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
