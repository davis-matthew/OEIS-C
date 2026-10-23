/*
 * A122588  Expansion of x/(1 - 9*x + 28*x^2 - 35*x^3 + 15*x^4 - x^5).
 * https://oeis.org/A122588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=1, a(2)=9, a(3)=53, a(4)=260, a(5)=1156; for n>5:
 *            a(n)=9*a(n-1)-28*a(n-2)+35*a(n-3)-15*a(n-4)+a(n-5)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    switch (n) {
        case 1: return 1;
        case 2: return 9;
        case 3: return 53;
        case 4: return 260;
        case 5: return 1156;
    }

    /* compute iteratively */
    seq_t a1 = 1;     /* a(n-5)   */
    seq_t a2 = 9;     /* a(n-4)   */
    seq_t a3 = 53;    /* a(n-3)   */
    seq_t a4 = 260;   /* a(n-2)   */
    seq_t a5 = 1156;  /* a(n-1)   */
    seq_t ak;         /* current term */

    for (idx_t i = 6; i <= n; ++i) {
        ak = ( (seq_t)9   * a5
             - (seq_t)28  * a4
             + (seq_t)35  * a3
             - (seq_t)15  * a2
             + a1 );
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ak;
    }
    return a5;
}
