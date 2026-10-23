/*
 * A045808  8-ish numbers (end in 67, 69, 81, 83).
 * https://oeis.org/A045808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100*((n-1)/4) + [67,69,81,83][(n-1)%4]
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n==0 is undefined, but we guard anyway */
    if (n == 0)
        return (seq_t)0;
    idx_t idx = n - 1;
    idx_t k = idx / 4;
    idx_t r = idx % 4;
    seq_t base = (seq_t)100 * (seq_t)k;
    switch (r) {
        case 0:
            return base + (seq_t)67;
        case 1:
            return base + (seq_t)69;
        case 2:
            return base + (seq_t)81;
        case 3:
            return base + (seq_t)83;
    }
    /* unreachable, but keeps the compiler happy */
    return base;
}
