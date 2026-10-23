/*
 * A128588  Expansion of g.f. x*(1+x+x^2)/(1-x-x^2).
 * https://oeis.org/A128588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=4; a(n)=a(n-1)+a(n-2) for n>=4 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;

    seq_t prevprev = 2;   /* a(2) */
    seq_t prev     = 4;   /* a(3) */
    seq_t cur      = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
