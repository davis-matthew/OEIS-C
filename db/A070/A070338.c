/*
 * A070338  a(n) = 2^n mod 33.
 * https://oeis.org/A070338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 33
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t modulus = (seq_t)33;
    seq_t base = (seq_t)2;
    seq_t result = (seq_t)1;
    idx_t e = n;
    while (e)
    {
        if (e & 1)
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        e >>= 1;
    }
    return result;
}
