/*
 * A204457  Odd numbers not divisible by 13.
 * https://oeis.org/A204457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 + 2*floor((n+5)/12) (n ≥ 1)
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Avoid unwanted behaviour for n < 1; the sequence is defined for n ≥ 1. */
    if (n == 0) return (seq_t)0;

    /* Compute the floor division (n+5)/12 using unsigned arithmetic. */
    idx_t t = (n + 5) / 12;

    /* Build the result using seq_t operations only. */
    seq_t part1 = (seq_t)2 * (seq_t)n - (seq_t)1;
    seq_t part2 = (seq_t)2 * (seq_t)t;
    return part1 + part2;
}
