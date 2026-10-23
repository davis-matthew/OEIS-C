/*
 * A016241  Expansion of 1/((1-x)*(1-6*x)*(1-7*x)).
 * https://oeis.org/A016241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13 * a(n-1) - 42 * a(n-2) + 1; a(0) = 1, a(1) = 14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)14;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)14;
    seq_t a2;

    for (idx_t i = 2u; i <= n; ++i) {
        a2 = (seq_t)13 * a1 - (seq_t)42 * a0 + (seq_t)1;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
