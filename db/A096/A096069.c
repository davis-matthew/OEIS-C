/*
 * A096069  Smallest prime ending in prime(n) and == 1 (mod prime(n)), or 0 if
 *          no such prime exists.
 * https://oeis.org/A096069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Uses CRT to find the smallest prime p such that p ≡ 1 (mod
 *            prime(n)) and p ends with the decimal representation of
 *            prime(n). Returns 0 if no such prime exists (n = 1 or 3)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 25, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   digits_of, get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) -> uint32_t */
#include "../utils/pow10.h"       /* pow10(idx_t e) -> seq_t */
#include "../utils/digits_of.h"   /* digits_of(idx_t x) -> idx_t */
#include "../utils/is_prime_big.h"/* is_prime_big(seq_t n) -> bool */

/* Extended Euclidean algorithm to compute modular inverse (a^-1 mod m).
   Assumes 1 <= a < m and gcd(a, m) == 1.  Returns the inverse modulo m. */
static uint64_t modinv_uint64(uint64_t a, uint64_t m) {
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a % m;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        int64_t tmp = t1;  t1 = t0 - q * t1;  t0 = tmp;
        tmp = r1;          r1 = r0 - q * r1;  r0 = tmp;
    }
    /* r0 is now gcd(a, m); for primes it should be 1 */
    if (r0 != 1)
        return 0;          /* inverse does not exist */
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

seq_t a(idx_t n) {
    uint32_t p = get_prime(n);          /* prime(n) */
    if (p == 2 || p == 5)               /* no solution for 2 or 5 */
        return 0;

    idx_t d = digits_of(p);             /* number of decimal digits of p */
    seq_t pow10d = pow10(d);            /* 10^d as seq_t */

    /* Compute 10^d mod p efficiently */
    uint64_t pow10_mod_p = 1;
    for (idx_t i = 0; i < d; ++i)
        pow10_mod_p = (pow10_mod_p * 10) % p;

    uint64_t a_mod = pow10_mod_p % p;   /* a in CRT equation */
    uint64_t b = 1 % p;                 /* RHS of CRT equation */
    uint64_t inv = modinv_uint64(a_mod, p);
    if (inv == 0)                       /* should not happen for p>5 */
        return 0;
    uint64_t t = (inv * b) % p;         /* smallest non-negative t */

    /* r = suffix + t * 10^d */
    seq_t r = (seq_t)p + (seq_t)t * pow10d;

    /* L = p * 10^d (common difference of the arithmetic progression) */
    seq_t L = (seq_t)p * pow10d;

    /* Search for the first prime in the progression starting from r */
    seq_t candidate = r;
    while (true) {
        if (candidate >= 2 && is_prime_big(candidate))
            return candidate;
        /* Stop if addition overflows seq_t (unlikely for reasonable n) */
        if (candidate + L < candidate)
            return 0;
        candidate += L;
    }
}
