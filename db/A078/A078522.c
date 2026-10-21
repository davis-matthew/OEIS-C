/*
 * A078522  Numbers k such that (k+1)*(2*k+1) is a perfect square.
 * https://oeis.org/A078522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) + 24, a(1)=0, a(2)=24
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 24;

    seq_t prev = 0;
    seq_t curr = 24;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = 34 * curr - prev + 24;
        prev = curr;
        curr = next;
    }
    return curr;
}
