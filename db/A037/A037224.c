/*
 * A037224  Number of permutations p of {1,2,3...,n} that are fixed points
 *          under the operation of first reversing p, then taking the inverse.
 * https://oeis.org/A037224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 1 * a(n) = 0 if n mod 4 = 2 or 3 * a(n) = 2 *
 *            Π_{i=k}^{2k-1} i if n mod 4 = 0 or 1 * where k = n/4 if n mod 4
 *            = 0 * k = (n-1)/4 if n mod 4 = 1 * (the product is empty when
 *            k=1 and equals 1)
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 35, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special first term */
    if (n == 1)
        return 1;

    /* zero for n = 4k+2 or 4k+3 */
    if ((n & 3) == 2 || (n & 3) == 3)
        return 0;

    /* compute k */
    idx_t k = (n & 3) == 0 ? n / 4 : (n - 1) / 4;

    /* result = 2 * product_{i=k}^{2k-1} i */
    seq_t result = 2;
    for (idx_t i = k; i <= 2 * k - 1; ++i) {
        result *= (seq_t)i;
    }

    return result;
}
