/*
 * A070364  a(n) = 3^n mod 47.
 * https://oeis.org/A070364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 47
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    seq_t mod    = (seq_t)47;

    /* Fast exponentiation by squaring */
    while (n)
    {
        if (n & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1ULL;
    }
    return result;
}
