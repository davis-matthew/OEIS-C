/*
 * A195616  Denominators of Pythagorean approximations to 3.
 * https://oeis.org/A195616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 37*a(n-1) + 37*a(n-2) - a(n-3), a(1) = 12, a(2) = 444,
 *            a(3) = 16872
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)            /* sequence starts at n = 1 */
        return (seq_t)0;

    if (n == 1u)
        return (seq_t)12;
    if (n == 2u)
        return (seq_t)444;
    if (n == 3u)
        return (seq_t)16872;

    /* use three variables to hold the last three terms */
    seq_t t1 = (seq_t)12;   /* a(n-3) */
    seq_t t2 = (seq_t)444;  /* a(n-2) */
    seq_t t3 = (seq_t)16872;/* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)37 * t3 + (seq_t)37 * t2 - t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
