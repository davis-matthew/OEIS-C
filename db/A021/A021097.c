/*
 * A021097  Decimal expansion of 1/93.
 * https://oeis.org/A021097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/93) mod 10 (decimal digits of 1/93)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/93 has period 15 in its decimal expansion:
       0 1 0 7 5 2 6 8 8 1 7 2 0 4 3 repeating. */
    static const seq_t cycle[15] = {
        0, 1, 0, 7, 5, 2, 6, 8, 8, 1, 7, 2, 0, 4, 3
    };
    return cycle[n % 15];
}
