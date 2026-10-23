/*
 * A170760  Expansion of g.f.: (1+x)/(1-40*x).
 * https://oeis.org/A170760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*40^(n-1) for n > 0, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)40;
    idx_t exp    = n - (idx_t)1;

    while (exp > 0) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    return result * (seq_t)41;
}
