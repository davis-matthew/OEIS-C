/*
 * A126605  Final three digits of 2^n.
 * https://oeis.org/A126605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^n mod 1000
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n modulo 1000 using fast exponentiation. */
    if (n == 0) {
        return (seq_t)1;
    }

    unsigned long long mod   = 1000ULL;
    unsigned long long result = 1ULL;
    unsigned long long base   = 2ULL;

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            result = (result * base) % mod;
        }
        base   = (base * base) % mod;
        exp   >>= 1ULL;
    }

    return (seq_t)result;
}
