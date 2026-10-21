/*
 * A070417  a(n) = 7^n mod 33.
 * https://oeis.org/A070417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 33
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce the exponent modulo the period of 7 modulo 33, which is 10 */
    idx_t r = n % 10;
    seq_t res = 1;

    for (idx_t i = 0; i < r; ++i)
    {
        /* (res * 7) mod 33 – res and 7 are at most 32, so the product fits
           safely into an unsigned long long before the modulo */
        res = (seq_t)(((unsigned long long)res * 7ull) % 33ull);
    }

    return res;
}
