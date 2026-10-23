/*
 * A026055  a(n) = (d(n)-r(n))/2, where d = A026054 and r is the periodic
 *          sequence with fundamental period (1,0,0,0).
 * https://oeis.org/A026055
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A026055
 *  Formula (offset 3): a(n) = ((m+2)*(m+3)*(m+13) - δ) / 12
 *      where m = n-3,
 *            δ = 6   if m mod 4 == 0
 *            δ = 0   otherwise
 *  This is equivalent to a(n) = (d(n)-r(n))/2 with the given definitions.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 3;                      /* shift offset */
    seq_t part1 = (seq_t)(m + 2);
    seq_t part2 = (seq_t)(m + 3);
    seq_t part3 = (seq_t)(m + 13);
    seq_t num   = part1 * part2 * part3;  /* numerator of the cubic term */

    /* Adjust for the periodic correction (δ = 6 when m ≡ 0 (mod 4)) */
    if ((m & 3u) == 0)    /* m % 4 == 0 */
        num -= (seq_t)6;

    return num / (seq_t)12;
}
