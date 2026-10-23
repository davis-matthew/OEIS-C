/*
 * A192350  Constant term of the reduction (by x^2->x+1) of polynomial p(n,x)
 *          identified in Comments.
 * https://oeis.org/A192350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+12*a(n-2)-8*a(n-3)-16*a(n-4); Initial values:
 *            a(1)=1, a(2)=0, a(3)=6, a(4)=4 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* below the offset: behaviour does not matter */
    if (n < 1)
        return 0;

    /* first four terms are hard‑coded for clarity */
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 6;
    if (n == 4) return 4;

    /* recurrence: keep the last four values only */
    seq_t a1 = 1; /* a(n-4) when n==5 */
    seq_t a2 = 0; /* a(n-3) */
    seq_t a3 = 6; /* a(n-2) */
    seq_t a4 = 4; /* a(n-1) */
    seq_t an;     /* a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        an = 2 * a4 + 12 * a3 - 8 * a2 - 16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
