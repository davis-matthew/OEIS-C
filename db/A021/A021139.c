/*
 * A021139  Decimal expansion of 1/135.
 * https://oeis.org/A021139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th decimal digit after the point of 1/135. It is
 *            computed by successive long‑division steps: starting with
 *            remainder r=1, for each step r*10 → quotient digit, new
 *            remainder
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1;   /* remainder before the first digit */
    seq_t digit = 0; /* current digit */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t prod = rem * (seq_t)10;
        digit = prod / (seq_t)135;
        rem   = prod % (seq_t)135;
    }
    return digit;
}
