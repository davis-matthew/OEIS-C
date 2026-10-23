/*
 * A011627  Legendre symbol (n,229).
 * https://oeis.org/A011627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,229) a(n) = n^114 (mod 229); a(0)=0,
 *            a(n)=1 if n mod 229 is a quadratic residue, a(n)=-1 otherwise.
 *            Period: 229
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed by the build */

static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Compute n modulo the prime 229 */
    const uint64_t p = 229ULL;          /* the prime modulus */
    uint64_t n_mod = (uint64_t)n % p;   /* reduce index to [0,p-1] */

    /* n mod 229 == 0  ->  a(n) = 0 */
    if (n_mod == 0)
        return (seq_t)0;

    /* Euler's criterion: a^((p-1)/2) mod p  ->  1 or p-1 (i.e. 228) */
    uint64_t exp = (p - 1) / 2;        /* 114 */
    uint64_t res = modpow_uint64(n_mod, exp, p);

    if (res == 1)
        return (seq_t)1;
    else  /* res == p-1 == 228 corresponds to -1 Legendre symbol */
        return (seq_t)-1;
}
