/*
 * A021641  Decimal expansion of 1/637.
 * https://oeis.org/A021641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 637) / 637)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the nth digit after the decimal point in 1/637.  */
#include <stdint.h>

/* Compute (base^exp mod mod) where base, mod < 2^32, exp fits in idx_t. */
static inline unsigned long long pow_mod(unsigned long long base,
                                         idx_t exp,
                                         unsigned long long mod)
{
    unsigned long long result = 1ULL % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 637 = 7^2 * 13, so the period of 1/637 is 42, but we compute
       the digit directly in O(log n) using modular exponentiation. */
    const unsigned long long MOD = 637ULL;
    /* r = 10^n mod 637 */
    unsigned long long r = pow_mod(10ULL, n, MOD);
    /* digit = floor(10 * r / 637) */
    unsigned long long digit = (r * 10ULL) / MOD;
    return (seq_t)digit;
}
