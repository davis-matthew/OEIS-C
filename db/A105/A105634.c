/*
 * A105634  Expansion of Sum_{k>0} Kronecker(k,7)*x^k*(1 + x^k)/(1 - x^k)^3.
 * https://oeis.org/A105634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A105634 – Expansion of Sum_{k>0} Kronecker(k,7)*x^k*(1 + x^k)/(1 - x^k)^3
 *
 *  The function implements the multiplicative definition:
 *
 *      a(p^e) = { 7^(2e)                                 if p = 7
 *               { (p^(2e+2)-1)/(p^2-1)                   if p ≡ 1,2,4 (mod 7)
 *               { (p^(2e+2)+(-1)^e)/(p^2+1)             if p ≡ 3,5,6 (mod 7)
 *
 *  a(n) for composite n is the product of a(p^e) over all prime powers in n.
 *
 *  The implementation uses trial division for factorisation; this is fast for
 *  the range of n used in the tests (up to about 10^4) and remains adequate
 *  for larger values as well.
 *
 *  No external libraries are required; all arithmetic uses the native
 *  seq_t type supplied by the build environment.
 */

#include <stdint.h>

/* Utility: fast exponentiation of a small base to an idx_t exponent.
 * Base is promoted to seq_t; result is seq_t. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Compute a(p^e) for a prime power. */
static seq_t a_prime_power(idx_t p, idx_t e)
{
    if (p == 7) {
        return pow_seq((seq_t)7, (idx_t)(2 * e));
    }

    idx_t r = p % 7ULL;          /* remainder modulo 7 */
    if (r == 1 || r == 2 || r == 4) { /* case (p^(2e+2)-1)/(p^2-1) */
        seq_t denom = pow_seq((seq_t)p, 2) - (seq_t)1;
        seq_t num   = pow_seq((seq_t)p, 2 * e + 2) - (seq_t)1;
        return num / denom;
    } else {                      /* case (p^(2e+2)+(-1)^e)/(p^2+1) */
        seq_t denom = pow_seq((seq_t)p, 2) + (seq_t)1;
        seq_t num   = pow_seq((seq_t)p, 2 * e + 2);
        if ((e & 1ULL) == 0) num += (seq_t)1;
        else                 num -= (seq_t)1;
        return num / denom;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but harmless */

    seq_t result = (seq_t)1;
    idx_t m = n;
    idx_t e = 0;

    /* Factor 2 separately for speed */
    if ((m & 1ULL) == 0) {
        e = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++e; }
        result *= a_prime_power(2, e);
    }

    /* Factor remaining odd primes */
    for (idx_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            e = 0;
            while (m % i == 0) { m /= i; ++e; }
            result *= a_prime_power(i, e);
        }
    }

    /* If a prime factor larger than sqrt(n) remains */
    if (m > 1) {
        result *= a_prime_power(m, 1);
    }

    return result;
}
