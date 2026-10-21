/*
 * A070720  a(n) = n^7 mod 39.
 * https://oeis.org/A070720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 39
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 39 (intuitively the exponentiation will never exceed 39) */
    unsigned long long nmod = n % 39;
    unsigned long long res = 1;

    /* Raise nmod to the 7th power, reducing at each step to keep values small */
    for (int i = 0; i < 7; ++i)
    {
        res = (res * nmod) % 39;
    }

    /* Convert back to the sequence type */
    return (seq_t)res;
}
