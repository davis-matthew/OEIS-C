/*
 * A062296  a(n) = number of entries in n-th row of Pascal's triangle
 *          divisible by 3.
 * https://oeis.org/A062296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n + 1 - ∏_{d∈base3(n)} (d + 1) Where the product is taken
 *            over the digits of n written in base 3. This counts the number
 *            of entries in row n of Pascal's triangle that are divisible by 3
 * Range:     u8 n <= 260, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    idx_t m = n;

    /* Handle n = 0 separately to ensure prod = 1 */
    if (m == 0) {
        prod = (seq_t)1;
    } else {
        while (m > 0) {
            idx_t digit = m % 3;          /* current base‑3 digit */
            prod *= ((seq_t)digit + (seq_t)1);
            m /= 3;
        }
    }

    /* a(n) = n + 1 - product of (digit+1) */
    return ((seq_t)n + (seq_t)1) - prod;
}
