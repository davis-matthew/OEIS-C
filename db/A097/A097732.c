/*
 * A097732  Pell equation solutions (7*a(n))^2 - 2*(5*b(n))^2 = -1 with
 *          b(n):=A097733(n), n >= 0. Note that D=50=2*5^2 is not squarefree.
 * https://oeis.org/A097732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 199, a(n) = 198*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)199;
    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)199;    /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)198 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
