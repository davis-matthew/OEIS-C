/*
 * A064692  Total number of holes in decimal expansion of the number n,
 *          assuming 4 has 1 hole.
 * https://oeis.org/A064692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = total number of closed loops in the decimal digits of n
 *            (digits 0,4,6,9 have 1 loop each, digit 8 has 2 loops, and all
 *            other digits have 0 loops). The sequence is indexed with a(0) =
 *            1
 * Range:     u8 n <= 50000, u16 n <= 50000, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 121 terms through n = 8906 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char digit_holes[10] = {
    1, /* 0 */
    0, /* 1 */
    0, /* 2 */
    0, /* 3 */
    1, /* 4 */
    0, /* 5 */
    1, /* 6 */
    0, /* 7 */
    2, /* 8 */
    1  /* 9 */
};

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */
    seq_t total = 0;
    while (n > 0)
    {
        total += digit_holes[n % 10];
        n /= 10;
    }
    return total;
}
