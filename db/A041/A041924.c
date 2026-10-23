/*
 * A041924  Numerators of continued fraction convergents to sqrt(485).
 * https://oeis.org/A041924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 44*a(n-1)+a(n-2), a(0) = 22, a(1) = 969
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;
    if (n == 1)
        return (seq_t)969;

    seq_t prev2 = (seq_t)22;   /* a(0) */
    seq_t prev1 = (seq_t)969;  /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = (seq_t)44 * prev1 + prev2;
        prev2   = prev1;
        prev1   = current;
    }
    return prev1;
}
