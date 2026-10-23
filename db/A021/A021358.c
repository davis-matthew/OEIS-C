/*
 * A021358  Decimal expansion of 1/354.
 * https://oeis.org/A021358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/354⌋ mod 10
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses repeated long division to find the nth decimal digit of 1/354. */
seq_t a(idx_t n)
{
    seq_t rem = 1;
    const seq_t denom = 354;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        seq_t d = rem / denom;
        rem %= denom;
        if (i == n) return d;
    }
    return 0; /* unreachable */
}
