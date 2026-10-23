/*
 * A135351  a(n) = (2^n + 3 - 7*(-1)^n + 3*0^n)/6; or a(0) = 0 and for n > 0,
 *          a(n) = A005578(n-1) - (-1)^n.
 * https://oeis.org/A135351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n + 3 - 7*(-1)^n + 3*0^n)/6 * Recurrence: a(0)=0,
 *            a(1)=2, a(2)=0, a(3)=3, * a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3)
 *            for n ≥ 4
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 0;
    if (n == 3) return 3;

    /* Keep last three values of the sequence */
    seq_t a1 = 2; /* a(n-3) */
    seq_t a2 = 0; /* a(n-2) */
    seq_t a3 = 3; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = 2 * a3 + a2 - 2 * a1; /* a(n) = 2*a(n-1)+a(n-2)-2*a(n-3) */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
