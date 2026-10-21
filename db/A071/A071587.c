/*
 * A071587  Powers of 9 written backwards.
 * https://oeis.org/A071587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = reverse decimal digits of 9ⁿ
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 9ⁿ */
    seq_t pow9 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow9 *= 9;

    /* Reverse the decimal digits of pow9 */
    seq_t rev = 0;
    while (pow9 != 0) {
        seq_t d = pow9 % 10;   /* least significant digit */
        pow9 /= 10;            /* remove that digit */
        rev = rev * 10 + d;    /* append it to the reversed number */
    }
    return rev;
}
