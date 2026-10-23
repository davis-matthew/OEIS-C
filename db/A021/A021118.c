/*
 * A021118  Decimal expansion of 1/114.
 * https://oeis.org/A021118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/114⌋ - 10⋅⌊10^n/114⌋
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the digits of the decimal expansion of 1/114. */
seq_t a(idx_t n)
{
    /* The first two digits (n = 0,1) are the integer part and the
       initial non‑repeating decimal digit.  The repeating cycle
       of length 18 starts at n = 2. */
    if (n <= 1u)
        return (seq_t)0;
    /* Pattern of the repeating part, starting at n = 2. */
    static const seq_t repeat[18] = {
        8, 7, 7, 1, 9, 2, 9, 8, 2, 4,
        5, 6, 1, 4, 0, 3, 5, 0
    };
    idx_t pos = (n - 2u) % 18u;
    return repeat[pos];
}
