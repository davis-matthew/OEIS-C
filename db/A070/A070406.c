/*
 * A070406  a(n) = 7^n mod 15.
 * https://oeis.org/A070406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 15
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long exp = n;          /* exponent */
    unsigned long long result = 1ULL;    /* 7^exp mod 15 */
    unsigned long long base   = 7ULL;     /* 7 mod 15 */
    const unsigned long long mod   = 15ULL;/* modulus */

    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return (seq_t)result;                /* cast back to seq_t */
}
