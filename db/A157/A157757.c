/*
 * A157757  a(n) = 2809*n^2 - 4618*n + 1898.
 * https://oeis.org/A157757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2809*n^2 - 4618*n + 1898
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1237, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 2809 * nn * nn;   /* 2809*n^2 */
    val -= 4618 * nn;             /* -4618*n */
    val += 1898;                  /* +1898 */
    return val;
}
