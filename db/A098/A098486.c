/*
 * A098486  Odd numbers with replacement of all prime factors 3 by 2.
 * https://oeis.org/A098486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   replace every factor 3 in the odd number (2n‑1) with 2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the nth odd number is 2*n – 1 */
    seq_t odd = (seq_t)n * 2 - 1;

    /* remove all factors of 3 and count them */
    seq_t temp = odd;
    int k = 0;
    while (temp % 3 == 0) {
        temp /= 3;
        ++k;
    }

    /* multiply by 2^k */
    seq_t pow2 = 1;
    while (k--) pow2 <<= 1;

    /* the result is (odd / 3^k) * 2^k */
    return temp * pow2;
}
