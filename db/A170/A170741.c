/*
 * A170741  Expansion of g.f.: (1+x)/(1-21*x).
 * https://oeis.org/A170741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 22 * 21^(n-1) for n > 0 The values are computed
 *            with binary exponentiation to keep the complexity O(log n). All
 *            arithmetic is performed in seq_t which may be an
 *            arbitrary-precision integer type or a fixed-width unsigned
 *            integer type
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)21;
    idx_t  exp   = n - 1U;

    while (exp > 0U) {
        if (exp & 1U) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1U;
    }

    return (seq_t)22 * result;
}
