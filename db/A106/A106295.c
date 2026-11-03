/*
 * A106295  Period of the Lucas 4-step sequence A073817 mod n.
 * https://oeis.org/A106295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( a(p_i^e_i) for all prime powers dividing n ) where
 *            a(2^k) = 5 * 2^(k-1) (k > 0) and for odd primes: if a(p) !=
 *            a(p^2) then a(p^k) = p^(k-1) * a(p) else a(p^k) = a(p)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 1290, u64 n <= 1486,
 *            u128 n <= 1486, big any n
 * Validated: standard, pass, 63 terms through n = 1474 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Assumes seq_t and idx_t are defined by the build.
    Uses 64‑bit arithmetic for indices and a simple Euclidean gcd for seq_t. */

#include <stdint.h>
#include <stddef.h>

/* Period of the Lucas‑4 sequence (initial vector 0,1,1,1) modulo mod (mod>1). */
static seq_t period_mod(uint64_t mod)
{
    /* the walk is in 64 bits (in seq_t it timed out under big) */
    if (mod <= 1) return (seq_t)1;  /* a(1) = 1 */
    /* initial state: a0=0,a1=1,a2=1,a3=1 */
    uint64_t s0 = 0, s1 = 1, s2 = 1, s3 = 1;
    uint64_t period = 0;
    while (1) {
        uint64_t nxt = (s0 + s1 + s2 + s3) % mod;
        s0 = s1;
        s1 = s2;
        s2 = s3;
        s3 = nxt;
        period++;
        if (s0 == 0 && s1 == 1 % mod && s2 == 1 % mod && s3 == 1 % mod)
            return (seq_t)period;
    }
}

/* Period for a prime power p^e.  p is odd or 2 handled explicitly. */
static seq_t period_prime_power(uint32_t p, unsigned e)
{
    if (p == 2) {
        /* a(2^k) = 5 * 2^(k-1) for k>0 */
        seq_t res = 5;
        for (unsigned i = 1; i < e; ++i)
            res <<= 1;      /* multiply by 2 */
        return res;
    }

    seq_t per1 = period_mod(p);
    if (e == 1)
        return per1;

    /* compute period for p^2 to decide behaviour */
    seq_t per2 = period_mod((uint64_t)p * (uint64_t)p);
    if (per2 == per1) {
        /* stays the same for higher powers */
        return per1;
    } else {
        /* a(p^k) = p^(k-1) * a(p) for k >= 1 */
        seq_t res = per1;
        for (unsigned i = 1; i < e; ++i)
            res *= (seq_t)p;
        return res;
    }
}

/* Euclidean gcd for seq_t (works for unsigned types of any size). */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n >= 1 */
    if (n == 0) return 0;    /* undefined, but return 0 */

    uint64_t rem = (uint64_t)n;
    seq_t result = 1;       /* lcm accumulator */

    /* factorization by trial division */
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            unsigned exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            seq_t per_p = period_prime_power((uint32_t)p, exp);
            seq_t g = gcd_seq(result, per_p);
            result = (result / g) * per_p;
        }
    }
    if (rem > 1) {          /* remaining prime factor */
        seq_t per_p = period_prime_power((uint32_t)rem, 1);
        seq_t g = gcd_seq(result, per_p);
        result = (result / g) * per_p;
    }

    return result;
}
