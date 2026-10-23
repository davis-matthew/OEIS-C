/*
 * A103847  McCarthy's 91 Function: a(n) = n-10 if n>100, otherwise a(n) =
 *          a(a(n+11)).
 * https://oeis.org/A103847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 91 for 1 ≤ n ≤ 101; otherwise a(n) = n - 10
 * Range:     u8 n <= 144, u16 n <= 144, u32 n <= 144, u64 n <= 144,
 *            u128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)101)
        return (seq_t)91;
    return (seq_t)(n - (idx_t)10);
}
