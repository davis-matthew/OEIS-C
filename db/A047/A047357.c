/*
 * A047357  Numbers that are congruent to {0, 1, 3} mod 7.
 * https://oeis.org/A047357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7k - 4 if n = 3k * a(n) = 7k - 6 if n = 3k - 1 * a(n) =
 *            7k - 7 if n = 3k - 2 * Equivalently: a(n) = a(n-1) + a(n-3) -
 *            a(n-4) for n>4, with * a(1)=0, a(2)=1, a(3)=3, a(4)=7. * The
 *            implementation below uses the closed‑form formula for O(1) time
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m, val;
    switch (n % 3) {
    case 0:          /* n = 3k */
        m = (seq_t)n / 3;
        val = 7 * m - (seq_t)4;
        break;
    case 1:          /* n = 3k - 2 */
        m = ((seq_t)n + 2) / 3;
        val = 7 * m - (seq_t)7;
        break;
    case 2:          /* n = 3k - 1 */
        m = ((seq_t)n + 1) / 3;
        val = 7 * m - (seq_t)6;
        break;
    default:
        val = 0;    /* unreachable */
        break;
    }
    return val;
}
