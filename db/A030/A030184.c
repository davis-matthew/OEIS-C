/*
 * A030184  Expansion of eta(q) * eta(q^3) * eta(q^5) * eta(q^15) in powers of
 *          q.
 * https://oeis.org/A030184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1. For prime powers: a(5^e) = 1, a(3^e) = (-1)^e,
 *            otherwise a(p^e) = a(p)*a(p^(e-1)) - p*a(p^(e-2)), where a(p) =
 *            -∑_{x=0}^{p-1} ((x+1)^2 + 4(x^3+x^2) | p) (Legendre symbol; p an
 *            odd prime. For p = 2, a(2) = -1)
 * Range:     i8 n <= 1800, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* modular exponentiation for arbitrary seq_t */
static seq_t pow_mod(seq_t base, seq_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* Legendre symbol (D|p) for odd prime p.  For p==2, return -1 for D≠0. */
static int legendre(seq_t D, seq_t p)
{
    if (p == 2) {
        if (D % p == 0) return 0;
        return -1;                /* by convention */
    }
    if (D % p == 0) return 0;
    seq_t ls = pow_mod(D % p, (p - 1) / 2, p);
    return (ls == p - 1) ? -1 : (int)ls;
}

/* Compute a(p) for odd prime p via Legendre symbol sum. */
static seq_t compute_a_prime(uint64_t p)
{
    seq_t sum = 0;
    for (seq_t x = 0; x < p; ++x) {
        seq_t x2   = (x * x) % p;
        seq_t x3   = (x2 * x) % p;
        seq_t rhs  = (x3 + x2) % p;            /* x^3 + x^2 */
        seq_t d    = ( ((x + 1) * (x + 1)) % p + (seq_t)4 * rhs ) % p;
        int ls     = legendre(d, p);
        sum += ls;
    }
    return -sum;
}

/* Cache a(p) for primes up to 100000. */
#define MAX_PRIME 100000
static seq_t a_prime_cache[MAX_PRIME + 1];
static unsigned char a_prime_valid[MAX_PRIME + 1];

/* Retrieve or compute a(p).  Handles p==2 specially. */
static seq_t get_a_prime(uint64_t p)
{
    if (p == 2) return -1;                      /* a(2) = -1 */
    if (p <= MAX_PRIME && a_prime_valid[p]) return a_prime_cache[p];
    seq_t val = compute_a_prime(p);
    if (p <= MAX_PRIME) {
        a_prime_cache[p] = val;
        a_prime_valid[p] = 1;
    }
    return val;
}

/* Prime‑power a(p^e) using the recurrence. */
static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    if (e == 0) return 1;                       /* p^0 = 1 */
    if (p == 5) return 1;
    if (p == 3) return ((e & 1) ? -1 : 1);
    seq_t ap     = get_a_prime(p);
    seq_t a_im2  = 1;                            /* a(p^0) */
    seq_t a_im1  = ap;                            /* a(p^1) */
    if (e == 1) return a_im1;
    seq_t a_i = 0;
    for (uint64_t i = 2; i <= e; ++i) {
        a_i = ap * a_im1 - (seq_t)p * a_im2;
        a_im2 = a_im1;
        a_im1 = a_i;
    }
    return a_i;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined outside offset */
    if (n == 1) return 1;

    /* Factor n by trial division (sufficient for n <= 10000). */
    uint64_t factors[16];
    uint64_t exps[16];
    size_t cnt = 0;
    uint64_t m = n;

    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        factors[cnt] = 2;
        exps[cnt] = e;
        ++cnt;
    }

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            factors[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        factors[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = factors[i];
        uint64_t e = exps[i];
        seq_t val = a_prime_power(p, e);
        result *= val;
    }
    return result;
}
