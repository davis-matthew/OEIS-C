/*
 * A178225  Characteristic function of A006995 (binary palindromes).
 * https://oeis.org/A178225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 if n is a binary palindrome, otherwise 0
 *            (characteristic function of A006995)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 is a palindrome by definition. */
    if (n == 0)
        return (seq_t)1;

    /* Compute the length of the binary representation of n. */
    unsigned len = 0;
    idx_t temp = n;
    while (temp) {
        len++;
        temp >>= 1;
    }

    /* Check that each bit i equals bit (len-1-i). */
    for (unsigned i = 0; i < len / 2; ++i) {
        idx_t low  = (n >> i) & 1;
        idx_t high = (n >> (len - 1 - i)) & 1;
        if (low != high)
            return (seq_t)0;
    }
    return (seq_t)1;
}
