/*
 * A077243  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; a(1) = 17; a(n) = 8*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)17;

    seq_t prev = (seq_t)2;      /* a(0) */
    seq_t curr = (seq_t)17;     /* a(1) */
    seq_t next;
    for (idx_t i = 2; i <= n; ++i) {
        next = (seq_t)(8 * curr - prev);
        prev = curr;
        curr = next;
    }
    return curr;
}
