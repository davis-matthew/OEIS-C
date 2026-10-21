/*
 * A055643  Babylonian numbers: integers in base 60 with each sexagesimal
 *          digit represented by 2 decimal digits, leading zeros omitted.
 * https://oeis.org/A055643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(60*n + r) = 100*a(n) + r, 0 <= r < 60
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 60)
        return (seq_t)n;
    else
        return 100 * a(n / 60) + (seq_t)(n % 60);
}
