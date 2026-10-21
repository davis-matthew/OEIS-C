/*
 * A091433  a(n) = 2*3^n - 18*4^n + 24*5^n.
 * https://oeis.org/A091433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n - 18*4^n + 24*5^n Recurrence: a(0)=8, a(1)=54,
 *            a(2)=330; a(n) = 12*a(n-1) - 47*a(n-2) + 60*a(n-3) for n ≥ 3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 8;
    if (n == 1) return 54;
    if (n == 2) return 330;

    seq_t a0 = 8;      /* a(n-3)   */
    seq_t a1 = 54;     /* a(n-2)   */
    seq_t a2 = 330;    /* a(n-1)   */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = 12 * a2 + 60 * a0;
        ai -= 47 * a1;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
