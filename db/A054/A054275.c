/*
 * A054275  Susceptibility series H_2 for 2-dimensional Ising model (divided
 *          by 2).
 * https://oeis.org/A054275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (11*n^2 + 4) / 2 for n > 0 and even a(n) =
 *            (11*n^2 + 5) / 2 for n > 0 and odd which is equivalent to the
 *            recurrence a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 4
 * Range:     u8 n <= 4, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;              /* n^2 */
    seq_t val  = (seq_t)11 * n2;       /* 11*n^2 */
    val += (n & 1) ? (seq_t)5 : (seq_t)4; /* +5 for odd, +4 for even */
    return val / (seq_t)2;
}
