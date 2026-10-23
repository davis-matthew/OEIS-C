/*
 * A026810  Number of partitions of n in which the greatest part is 4.
 * https://oeis.org/A026810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 4; otherwise round((n³ + 3·n² - 9·n*(n mod
 *            2))/144)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 2600, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * nn + 3 * nn * nn;   /* n³ + 3·n² */

    if ((n & 1U) != 0)          /* n is odd */
        num -= 9 * nn;          /* subtract 9·n */

    return (num + 72) / 144;    /* rounding: +½·144 before integer division */
}
