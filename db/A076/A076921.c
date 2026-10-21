/*
 * A076921  Smallest number such that the highest common factor of pair of
 *          successive terms follows the pattern 1, 1, 2, 2, 3, 3, 4, 4, ...,
 *          i.e., b(2n-1) = b(2n) = n given by A004526.
 * https://oeis.org/A076921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; * For n > 2 a(n) = (n even ? n²/4 : (n²-1)/4)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = (2*n² + (-1)ⁿ - 1)/8 for n > 1 */
seq_t a(idx_t n)
{
    if (n <= 2) return 1;

    seq_t nn = (seq_t)n;              /* promote n to seq_t for safe arithmetic */

    if ((n & 1) == 0) {                /* n is even:  a(n) = n² / 4 */
        return (nn * nn) / 4;
    } else {                            /* n is odd :  a(n) = (n² - 1) / 4 */
        return ((nn * nn) - 1) / 4;
    }
}
