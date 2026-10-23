/*
 * A106284  Primes p such that the polynomial x^5-x^4-x^3-x^2-x-1 mod p has no
 *          zeros.
 * https://oeis.org/A106284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p for which x^5−x^4−x^3−x^2−x−1 has no zero
 *            modulo p
 * Range:     u8 n <= 21, u16 n <= 2393, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mulmod64.h"

/* ---------- modular arithmetic helpers ---------- */

static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1) res = mulmod64(res, base, mod);
        base = mulmod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* ---------- polynomial over Fp (degree <=4) ---------- */
typedef struct { uint64_t c[6]; } Poly;   /* c[i] is coefficient of x^i */

static inline int poly_degree(const Poly *P)
{
    for (int i = 5; i >= 0; --i)
        if (P->c[i] != 0) return i;
    return -1;  /* zero polynomial */
}

static Poly mulmod_poly(const Poly *A, const Poly *B, uint64_t mod)
{
    unsigned __int128 tmp[9] = {0};

    for (int i = 0; i < 5; ++i)
        if (A->c[i])
            for (int j = 0; j < 5; ++j)
                if (B->c[j])
                    tmp[i + j] += (unsigned __int128)A->c[i] * B->c[j];

    for (int k = 8; k >= 5; --k)
    {
        uint64_t c = (uint64_t)(tmp[k] % mod);
        if (!c) continue;
        tmp[k - 1] += c;
        tmp[k - 2] += c;
        tmp[k - 3] += c;
        tmp[k - 4] += c;
        tmp[k - 5] += c;
        tmp[k] = 0;
    }

    Poly R;
    for (int i = 0; i < 5; ++i) R.c[i] = (uint64_t)(tmp[i] % mod);
    R.c[5] = 0;
    return R;
}

static Poly poly_pow_mod(Poly base, uint64_t exp, uint64_t mod)
{
    Poly res;
    for (int i = 0; i < 6; ++i) res.c[i] = 0;
    res.c[0] = 1 % mod;          /* constant 1 */

    while (exp)
    {
        if (exp & 1) res = mulmod_poly(&res, &base, mod);
        base     = mulmod_poly(&base, &base, mod);
        exp >>= 1;
    }
    return res;
}

static Poly poly_mod(const Poly *A, const Poly *B, uint64_t mod)
{
    Poly R = *A;
    int degB = poly_degree(B);
    if (degB == -1)
    {
        Poly zero;
        for (int i = 0; i < 6; ++i) zero.c[i] = 0;
        return zero;
    }

    uint64_t invLeadB = modpow64(B->c[degB], mod - 2, mod);
    int degR = poly_degree(&R);

    while (degR >= degB)
    {
        int shift = degR - degB;
        uint64_t c = mulmod64(R.c[degR], invLeadB, mod);

        for (int i = 0; i <= degB; ++i)
        {
            int idx = i + shift;
            uint64_t sub = mulmod64(c, B->c[i], mod);
            R.c[idx] = (R.c[idx] + mod - sub) % mod;
        }
        degR = poly_degree(&R);
    }
    return R;
}

static Poly poly_gcd(Poly A, Poly B, uint64_t mod)
{
    while (poly_degree(&B) >= 0)
    {
        Poly R = poly_mod(&A, &B, mod);
        A = B;
        B = R;
    }
    return A;
}

/* ---------- test whether a prime has no root of the polynomial ---------- */
static bool no_root_in_mod_prime(uint64_t p)
{
    /* f(x) = x^5 - x^4 - x^3 - x^2 - x - 1 mod p */
    Poly f;
    f.c[0] = p - 1;
    f.c[1] = p - 1;
    f.c[2] = p - 1;
    f.c[3] = p - 1;
    f.c[4] = p - 1;
    f.c[5] = 1;

    Poly xpoly;
    for (int i = 0; i < 6; ++i) xpoly.c[i] = 0;
    xpoly.c[1] = 1;

    /* g(x) = x^p mod f(x) */
    Poly g = poly_pow_mod(xpoly, p, p);

    /* h(x) = g(x) - x */
    Poly h = g;
    h.c[1] = (h.c[1] + p - 1) % p;

    /* gcd(f, h) */
    Poly d = poly_gcd(f, h, p);
    return poly_degree(&d) < 1;   /* constant or zero polynomial => no root */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static state: list of good primes and a simple prime cache */
    static seq_t *good      = NULL;
    static size_t good_cap  = 0;
    static size_t good_sz   = 0;

    static uint64_t *primes  = NULL;
    static size_t primes_cap = 0;
    static size_t primes_sz  = 0;

    static uint64_t next_candidate = 3;   /* next odd number to test */

    /* initialise prime cache with 2 */
    if (primes_sz == 0)
    {
        primes_cap = 4;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0]   = 2;
        primes_sz   = 1;
    }

    /* extend good list until we have n terms */
    while (good_sz < n)
    {
        /* primality test using cached primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_sz; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > next_candidate) break;
            if (next_candidate % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* cache prime for future divisions */
            if (primes_sz == primes_cap)
            {
                primes_cap = 2 * primes_cap;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_sz++] = next_candidate;

            /* test polynomial root condition */
            if (no_root_in_mod_prime(next_candidate))
            {
                /* cache good prime */
                if (good_sz == good_cap)
                {
                    good_cap = 2 * good_cap + 1;
                    good = (seq_t *)realloc(good, good_cap * sizeof(seq_t));
                }
                good[good_sz++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;      /* next odd number */
    }

    return good[n - 1];
}
