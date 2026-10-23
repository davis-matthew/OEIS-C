/*
 * A118112  a(n) = binomial(3n,n) mod (n+1).
 * https://oeis.org/A118112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3n,n) mod (n+1) (offset 1)
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_p_factorial, fact_no_p_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/exp_p_factorial.h"
#include "../utils/fact_no_p_mod.h"

/* extended Euclidean algorithm for unsigned 64‑bit, returns inverse of a mod m  (gcd(a,m)=1) */
static uint64_t
inv_mod_uint64(uint64_t a, uint64_t m)
{
    int64_t aa = a, bb = m;
    int64_t x0 = 1, x1 = 0, y0 = 0, y1 = 1;

    while (bb != 0) {
        int64_t q = aa / bb;
        int64_t t = aa - q * bb; aa = bb; bb = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
        t = y0 - q * y1; y0 = y1; y1 = t;
    }
    if (x0 < 0) x0 += m;
    return (uint64_t)x0;
}

/* modular exponentiation: base^exp mod mod, mod > 1 */
static uint64_t
pow_uint64(uint64_t base, unsigned exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* binomial(N,K) mod p^e  (N=3n, K=n)  using the method of removing factor p^e_needed */
static uint64_t
binom_mod_prime_power(unsigned N, unsigned K, unsigned p, unsigned e)
{
    unsigned e_needed = exp_p_factorial(N, p) -
                        exp_p_factorial(K, p) -
                        exp_p_factorial(N - K, p);
    if (e_needed >= e) return 0;

    /* modulus p^e */
    uint64_t mp = 1;
    for (unsigned i = 0; i < e; ++i) mp *= (uint64_t)p;

    uint64_t a = fact_no_p_mod(N, p, mp);
    uint64_t b = fact_no_p_mod(K, p, mp);
    uint64_t c = fact_no_p_mod(N - K, p, mp);

    uint64_t inv_b = inv_mod_uint64(b, mp);
    uint64_t inv_c = inv_mod_uint64(c, mp);

    uint64_t res = a;
    res = (res * inv_b) % mp;
    res = (res * inv_c) % mp;

    if (e_needed > 0) {
        uint64_t factor = pow_uint64(p, e_needed, mp);
        res = (res * factor) % mp;
    }
    return res;
}

/* combine current x ≡ a (mod modX) with y ≡ b (mod mp) using CRT  (modX and mp are coprime) */
static void
crt_combine(uint64_t *x, uint64_t *modX, uint64_t mp, uint64_t y)
{
    uint64_t inv = inv_mod_uint64(*modX % mp, mp);
    uint64_t t = ((y + mp - (*x % mp)) % mp * inv) % mp;
    *x += (*modX) * t;
    *modX *= mp;
    *x %= *modX;
}

/* sequence function: a(n) = binomial(3n,n) mod (n+1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not used, safety */

    /* modulus m = n+1 */
    unsigned m = (unsigned)(n + 1);
    /* factor m into primes^exps */
    unsigned primes[16];
    unsigned exps[16];
    unsigned pCnt = 0;
    unsigned tmp = m;
    for (unsigned p = 2; (uint64_t)p * p <= (uint64_t)tmp; ++p) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pCnt] = p;
            exps[pCnt]   = e;
            ++pCnt;
        }
    }
    if (tmp > 1) {
        primes[pCnt] = tmp;
        exps[pCnt]   = 1;
        ++pCnt;
    }

    unsigned N = 3 * (unsigned)n;
    unsigned K = (unsigned)n;

    uint64_t result = 0;
    uint64_t curMod = 1;

    for (unsigned i = 0; i < pCnt; ++i) {
        uint64_t mp = 1;
        for (unsigned j = 0; j < exps[i]; ++j) mp *= (uint64_t)primes[i];
        uint64_t residue = binom_mod_prime_power(N, K, primes[i], exps[i]);
        crt_combine(&result, &curMod, mp, residue);
    }

    return (seq_t)result;
}
