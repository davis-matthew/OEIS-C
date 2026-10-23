/*
 * A118107  Period of the vector sequence d(n)^2^k mod n for k=1,2,3,...,
 *          where d(n) is the vector of divisors of n.
 * https://oeis.org/A118107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of the vector sequence d(n)^2^k (mod n), k ≥ 1
 * Range:     u8 n <= 1173, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   gcd, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is defined by taking all divisors d of n, forming the
    vector (d^(2^k) mod n) for k=1,2,3,… and determining its eventual
    period.  The following implementation computes the period exactly
    for any n ≥ 1, using only integer arithmetic and standard utilities. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/powmod64.h"   /* power modulo 64‑bit (base,exp,mod)  */
#include "../utils/gcd.h"        /* gcd(seq_t,seq_t) */

/* --------------------------------------------------------------------- */
/* Helper: power of a uint64_t base to an exponent (small)                */
static uint64_t pow_uint64(uint64_t base, int exp)
{
    uint64_t res = 1;
    while (exp > 0) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Helper: factorise a uint64_t into distinct prime factors.              */
static void factor_uint64(uint64_t n, uint64_t *pr, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            pr[c++] = i;
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) pr[c++] = n;
    *cnt = c;
}

/* General multiplicative order of a modulo mod,  gcd(a,mod)=1.            */
static uint64_t order_mod(uint64_t a, uint64_t mod)
{
    if (mod == 1) return 1;
    /* compute φ(mod) */
    uint64_t phi = mod;
    uint64_t factors[20];
    size_t fc = 0;
    uint64_t temp = mod;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            phi /= p;
            phi *= (p - 1);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {
        phi /= temp;
        phi *= (temp - 1);
    }

    /* factor φ(mod) */
    uint64_t phi_factors[20];
    size_t pf = 0;
    factor_uint64(phi, phi_factors, &pf);

    uint64_t ord = phi;
    a %= mod;
    for (size_t i = 0; i < pf; ++i) {
        uint64_t q = phi_factors[i];
        while (ord % q == 0 && powmod64(a, ord / q, mod) == 1)
            ord /= q;
    }
    return ord;
}

/* LCM of two seq_t numbers.  The result is guaranteed to be an integer. */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0) return 0;
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/* Generate all divisors of n given its factorisation.                   */
static void gen_divisors_rec(const uint64_t *pr, const int *ex, int idx,
                             uint64_t cur, uint64_t *divs, size_t *cnt,
                             int tot)
{
    if (idx == tot) {
        divs[*cnt] = cur;
        (*cnt)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t mult = 1;
    for (int e = 0; e <= ex[idx]; ++e) {
        gen_divisors_rec(pr, ex, idx + 1, cur * mult,
                         divs, cnt, tot);
        mult *= p;
    }
}

/* --------------------------------------------------------------------- */
/* Main function as required by the spec.                                 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset starts at 1 */

    /* 1. Factorise n into primes and exponents */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[20];
    int exp[20];
    int num = 0;
    uint64_t tmp = nn;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            primes[num] = p;
            exp[num] = e;
            num++;
        }
    }
    if (tmp > 1) {
        primes[num] = tmp;
        exp[num] = 1;
        num++;
    }

    /* 2. Enumerate all divisors of n */
    uint64_t divisors[2000];
    size_t dcnt = 0;
    gen_divisors_rec(primes, exp, 0, 1, divisors, &dcnt, num);

    /* 3. Compute the overall period as lcm over all divisors */
    seq_t L = 1;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        seq_t period_d = 1;

        for (int j = 0; j < num; ++j) {
            uint64_t p = primes[j];
            int e = exp[j];
            if (d % p == 0) continue;          /* component becomes 0 eventually */

            uint64_t mod_p = pow_uint64(p, e);
            uint64_t d_mod = d % mod_p;

            /* order of d_mod modulo mod_p (p^e) */
            uint64_t m = order_mod(d_mod, mod_p);

            /* remove all 2‑factors from m */
            uint64_t m_odd = m;
            while ((m_odd & 1) == 0) m_odd >>= 1;

            /* period contributed by this prime */
            uint64_t t;
            if (m_odd == 1) t = 1;
            else             t = order_mod(2, m_odd);

            period_d = lcm_seq(period_d, (seq_t)t);
            if (period_d == 1) continue;        /* cannot increase further */
        }
        L = lcm_seq(L, period_d);
    }
    return L;
}
