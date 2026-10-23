/*
 * A135844  Prime numbers p for which the quintic polynomial x^5 - x - 1
 *          modulo p completely factors into linear polynomials.
 * https://oeis.org/A135844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p for which x^5 - x - 1 modulo p
   completely splits into linear factors.  The routine
   a(n) returns the n‑th such prime (OEIS offset = 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod64.h"
#include "../utils/next_prime.h"

/* polynomial type – degree ≤ 5 */
typedef struct {
    uint64_t c[6];          /* coefficients 0 … 5 (mod p) */
} poly_t;

/* degree of a polynomial (returns -1 for zero polynomial) */
static int poly_deg(const poly_t *p, uint64_t mod)
{
    for (int i = 5; i >= 0; --i) {
        if (p->c[i] % mod) return i;
    }
    return -1;
}

/* multiplication of two polynomials modulo f(x)=x^5-x-1 (mod p) */
static void poly_mul_mod(poly_t *res, const poly_t *a, const poly_t *b, uint64_t mod)
{
    uint64_t tmp[11];
    memset(tmp, 0, sizeof(tmp));

    /* ordinary polynomial multiplication (degree ≤ 10) */
    for (int i = 0; i <= 5; ++i) {
        if (!a->c[i]) continue;
        for (int j = 0; j <= 5; ++j) {
            if (!b->c[j]) continue;
            uint64_t prod = (a->c[i] * b->c[j]) % mod;
            uint64_t sum  = tmp[i + j] + prod;
            if (sum >= mod) sum -= mod;
            tmp[i + j] = sum;
        }
    }

    /* reduction using x^5 ≡ x + 1 (mod p) */
    for (int deg = 10; deg >= 5; --deg) {
        uint64_t coeff = tmp[deg] % mod;
        if (!coeff) continue;
        /* x^deg = x^{deg-5}*(x+1) = x^{deg-4} + x^{deg-5} */
        tmp[deg - 5] = (tmp[deg - 5] + coeff) % mod;
        tmp[deg - 4] = (tmp[deg - 4] + coeff) % mod;
        tmp[deg] = 0;
    }

    for (int i = 0; i <= 5; ++i) res->c[i] = tmp[i] % mod;
}

/* remainder of poly division (a = q·b + r); only r is returned */
static void poly_mod(poly_t *r, const poly_t *a, const poly_t *b, uint64_t mod)
{
    poly_t temp;
    memcpy(&temp, a, sizeof(temp));
    int db = poly_deg(b, mod);
    if (db < 0) {
        memset(r, 0, sizeof(temp));
        return;
    }

    while (1) {
        int da = poly_deg(&temp, mod);
        if (da < db) break;

        uint64_t inv = powmod64(b->c[db], mod - 2, mod);
        uint64_t factor = (temp.c[da] * inv) % mod;
        int shift = da - db;
        for (int i = db; i >= 0; --i) {
            int idx = shift + i;
            uint64_t sub = (factor * b->c[i]) % mod;
            if (temp.c[idx] >= sub)
                temp.c[idx] -= sub;
            else
                temp.c[idx] += mod - sub;
        }
    }
    memcpy(r, &temp, sizeof(temp));
}

/* Euclidean algorithm for polynomial GCD (mod p) */
static void poly_gcd(poly_t *res, poly_t a, poly_t b, uint64_t mod)
{
    poly_t r;
    while (poly_deg(&b, mod) >= 0) {
        poly_mod(&r, &a, &b, mod);
        a = b;
        b = r;
    }
    memcpy(res, &a, sizeof(a));
}

/* dynamic array helper */
static void grow_array(size_t *cap, void **ptr, size_t need, size_t elem)
{
    if (need <= *cap) return;
    size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
    while (newcap < need) newcap <<= 1;
    *ptr = realloc(*ptr, newcap * elem);
    *cap = newcap;
}

/* check whether a prime p makes x^5 - x - 1 split completely mod p */
static bool prime_has_property(uint64_t p)
{
    poly_t f, fdr, d, base, res, temp, h;
    /* f(x) = x^5 - x - 1  (coeffs mod p) */
    f.c[0] = (p - 1) % p;          /* -1 */
    f.c[1] = (p - 1) % p;          /* -1 */
    f.c[2] = f.c[3] = f.c[4] = 0;
    f.c[5] = 1 % p;

    /* f'(x) = 5x^4 - 1  (coeffs mod p) */
    fdr.c[0] = (p - 1) % p;        /* -1 */
    fdr.c[4] = 5 % p;
    fdr.c[1] = fdr.c[2] = fdr.c[3] = fdr.c[5] = 0;

    /* separability test: gcd(f, f') must be 1 */
    poly_gcd(&d, f, fdr, p);
    if (poly_deg(&d, p) > 0) return false;

    /* compute x^p mod f  via binary exponentiation */
    memset(&base, 0, sizeof(base));
    base.c[1] = 1 % p;          /* x */
    memset(&res, 0, sizeof(res));
    res.c[0] = 1 % p;           /* 1 */
    uint64_t exp = p;
    while (exp) {
        if (exp & 1) {
            poly_mul_mod(&temp, &res, &base, p);
            memcpy(&res, &temp, sizeof(temp));
        }
        poly_mul_mod(&temp, &base, &base, p);
        memcpy(&base, &temp, sizeof(temp));
        exp >>= 1;
    }

    /* h(x) = (x^p mod f) – x */
    memcpy(&h, &res, sizeof(res));
    if (h.c[1] >= 1) h.c[1] -= 1;
    else h.c[1] = (h.c[1] + p - 1) % p;

    /* f must divide h for complete splitting */
    poly_gcd(&d, f, h, p);
    return poly_deg(&d, p) == 5;
}

/* Return the n-th prime p for which x^5 - x - 1 splits completely
   modulo p.  (OEIS offset = 1). */
seq_t a(idx_t n)
{
    /* memoised results */
    static seq_t *results = NULL;
    static size_t results_cap = 0;
    static size_t results_len = 0;

    while (results_len < (size_t)n) {
        /* find next prime candidate */
        static uint64_t candidate = 2;
        candidate = next_prime(candidate);
        if (prime_has_property(candidate)) {
            grow_array(&results_cap, (void **)&results, results_len + 1, sizeof(seq_t));
            results[results_len++] = (seq_t)candidate;
        }
        ++candidate;  /* ensure next prime will be > current */
    }
    return results[n - 1];
}
