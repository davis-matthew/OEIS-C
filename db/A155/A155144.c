/*
 * A155144  a(n) = 9*a(n-1) + 9*a(n-2), n>2; a(0)=1, a(1)=8, a(2)=80.
 * https://oeis.org/A155144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=80, a(n)=9*a(n-1)+9*a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
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
        return (seq_t)8;
    if (n == 2)
        return (seq_t)80;

    seq_t prevprev = (seq_t)8;   /* a(1) */
    seq_t prev     = (seq_t)80;  /* a(2) */
    seq_t cur      = prev;       /* placeholder for a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)9 * prev + (seq_t)9 * prevprev;
        prevprev = prev;
        prev = cur;
    }

    return cur;
}
