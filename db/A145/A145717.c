/*
 * A145717  Numbers k such that there exists x in N with (x+127)^3-x^3=k^2.
 * https://oeis.org/A145717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2030*a(n-1) - a(n-2) with initial values a(1)=16129,
 *            a(2)=32725741. This recurrence generates OEIS sequence A145717
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined below offset */
    if (n == 1) return (seq_t)16129;
    if (n == 2) return (seq_t)32725741;

    seq_t prevprev = (seq_t)16129;
    seq_t prev     = (seq_t)32725741;
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)2030 * prev - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
