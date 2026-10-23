/*
 * A029698  Number of words of length 2n in the 10 transpositions of S[5]
 *          equivalent to the identity.
 * https://oeis.org/A029698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*4^n + 16*25^n + 100^n)/60 Recurrence (order 3):
 *            a(1)=10, a(2)=340, a(3)=20860 for n>=4:
 *            a(n)=129*a(n-1)-3000*a(n-2)+10000*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1 – return 0 for n==0 */
        return (seq_t)0;

    /* initialise the first three values */
    seq_t a1 = (seq_t)10;      /* a(1) */
    seq_t a2 = (seq_t)340;     /* a(2) */
    seq_t a3 = (seq_t)20860;   /* a(3) */

    /* handle small n directly */
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* iterative recurrence for n >= 4 */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t a4 = (seq_t)129 * a3
                 - (seq_t)3000 * a2
                 + (seq_t)10000 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
