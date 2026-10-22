/*
 * A000180  Expansion of E.g.f. exp(-x)/(1-3x).
 * https://oeis.org/A000180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*n*a(n-1) + (-1)^n, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        r = r * (seq_t)k * 3;
        if (k & 1)
            r -= 1;
        else
            r += 1;
    }
    return r;
}
