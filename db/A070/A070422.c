/*
 * A070422  a(n) = 7^n mod 39.
 * https://oeis.org/A070422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 39
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Binary exponentiation modulo 39 */
    const seq_t base = (seq_t)7;
    const seq_t mod  = (seq_t)39;
    seq_t result = (seq_t)1;
    seq_t b = base;
    idx_t e = n;

    while (e > 0)
    {
        if (e & (idx_t)1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        e >>= 1;
    }
    return result;
}
