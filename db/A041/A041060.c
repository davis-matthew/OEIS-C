/*
 * A041060  Numerators of continued fraction convergents to sqrt(37).
 * https://oeis.org/A041060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 12*a(n-1) + a(n-2), for n > 1; initial values: a(0) = 6,
 *            a(1) = 73
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)73;

    seq_t prev = (seq_t)6;   /* a(0) */
    seq_t curr = (seq_t)73;  /* a(1) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t next = (seq_t)12 * curr + prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
