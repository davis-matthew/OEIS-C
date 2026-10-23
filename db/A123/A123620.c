/*
 * A123620  Expansion of (1 + x + x^2) / (1 - 3*x - 3*x^2).
 * https://oeis.org/A123620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=16, a(n)=3*a(n-1)+3*a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 16;

    seq_t prevPrev = 4;  /* a(1) */
    seq_t prev     = 16; /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (3 * prev) + (3 * prevPrev);
        prevPrev = prev;
        prev     = curr;
    }
    return curr;
}
