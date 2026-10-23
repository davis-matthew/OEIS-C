/*
 * A016238  Expansion of 1/((1-x)*(1-5*x)*(1-11*x)).
 * https://oeis.org/A016238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 17; a(n) = 16*a(n-1) - 55*a(n-2) + 1 (n>1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)17;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)17;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev * (seq_t)16;
        cur -= prevprev * (seq_t)55;
        cur += (seq_t)1;
        prevprev = prev;
        prev = cur;
    }

    return prev;
}
