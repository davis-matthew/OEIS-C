/*
 * A154118  Expansion of (1 - x + 5x^2)/((1-x)*(1-2x)).
 * https://oeis.org/A154118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 7*2^(n-1) - 5, n>=1; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* For n >= 1 we compute the recurrence:
       a(1) = 2
       a(n) = 2*a(n-1) + 5, n > 1
       This is equivalent to the closed form above. */
    seq_t cur = (seq_t)2;          /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        cur = cur * (seq_t)2 + (seq_t)5;
    }

    return cur;
}
