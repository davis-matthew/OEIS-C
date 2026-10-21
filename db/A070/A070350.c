/*
 * A070350  a(n) = 2^n mod 45.
 * https://oeis.org/A070350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 45
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 1;
    seq_t base  = 2;
    seq_t mod   = (seq_t)45;
    idx_t e = n;

    while (e)
    {
        if (e & 1U)
            res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1U;
    }
    return res;
}
