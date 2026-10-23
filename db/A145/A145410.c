/*
 * A145410  Number of 2-factors in K_6 X P_n.
 * https://oeis.org/A145410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=70, a(2)=24400, a(3)=6912340, a(4)=1997380720;
 *            a(n)=264*a(n-1)+7160*a(n-2)-31008*a(n-3)-10480*a(n-4) for n>=5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 70U;
    if (n == 2) return 24400U;
    if (n == 3) return 6912340U;
    if (n == 4) return 1997380720U;

    seq_t a1 = 70U;
    seq_t a2 = 24400U;
    seq_t a3 = 6912340U;
    seq_t a4 = 1997380720U;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = a4 * 264U;
        an += a3 * 7160U;
        an -= a2 * 31008U;
        an -= a1 * 10480U;

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
