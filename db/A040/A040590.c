/*
 * A040590  Continued fraction for sqrt(615).
 * https://oeis.org/A040590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(n) = 24 if n==0; 1 if n odd; 48 if n even and divisible by 4;
 *            3 otherwise
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative function described in OEIS A040590 */
seq_t a(idx_t n)
{
    if (n == 0)          /* special value at offset 0 */
        return 24;
    if (n & 1)           /* n is odd */
        return 1;
    if ((n & 3) == 0)    /* n divisible by 4 */
        return 48;
    return 3;            /* n even but not divisible by 4 */
}
