/*
 * A106276  Number of distinct zeros of x^3-x^2-x-1 mod prime(n).
 * https://oeis.org/A106276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, pow_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A106276: number of distinct zeros of x^3-x^2-x-1 modulo the n‑th prime.
    The algorithm uses the identity   f(x)=x^3-x^2-x-1
    and computes the gcd of f and (x^p-x) over the field F_p.
    The degree of that gcd equals the number of distinct roots.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* seq_t get_prime(uint64_t idx); */
#include "../utils/pow_mod.h"     /* uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t m); */

/* polynomial of degree ≤ 3 : c[0] + c[1]x + c[2]x^2 + c[3]x^3   */
typedef struct { uint64_t c[4]; } poly;

/* degree of a polynomial, or -1 if zero */
static int poly_degree(const poly *p)
{
    int d = 3;
    while (d > 0 && p->c[d] == 0) d--;
    if (d == 0 && p->c[0] == 0) return -1; /* zero polynomial */
    return d;
}

/* modular inverse: a^(mod-2) mod mod, mod is prime */
static uint64_t mod_inv(uint64_t a, uint64_t mod)
{
    return pow_mod(a, mod - 2, mod);
}

/* multiply two degree ≤2 polynomials and reduce modulo x^3 - x^2 - x - 1 */
static poly mul_mod_f(const poly *a, const poly *b, uint64_t mod)
{
    uint64_t r[5] = {0,0,0,0,0};
    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
            r[i + j] = (r[i + j] + a->c[i] * b->c[j]) % mod;

    /* reduction using x^3 = x^2 + x + 1  (mod f) */
    poly res;
    res.c[0] = (r[0] + r[3] + r[4]) % mod;
    res.c[1] = (r[1] + r[3] + 2 * r[4]) % mod;
    res.c[2] = (r[2] + r[3] + 2 * r[4]) % mod;
    res.c[3] = 0;
    return res;
}

/* exponentiate x mod f: compute x^p (mod f) */
static poly pow_x_mod_f(uint64_t p, uint64_t mod)
{
    poly result = {{1,0,0,0}};
    poly base   = {{0,1,0,0}};
    while (p) {
        if (p & 1)
            result = mul_mod_f(&result, &base, mod);
        base = mul_mod_f(&base, &base, mod);
        p >>= 1;
    }
    return result;
}

/* subtraction of two polynomials modulo mod */
static poly poly_sub(const poly *a, const poly *b, uint64_t mod)
{
    poly r;
    r.c[0] = (a->c[0] + mod - b->c[0]) % mod;
    r.c[1] = (a->c[1] + mod - b->c[1]) % mod;
    r.c[2] = (a->c[2] + mod - b->c[2]) % mod;
    r.c[3] = (a->c[3] + mod - b->c[3]) % mod;
    return r;
}

/* polynomial remainder: a % b  (-> rem).  assumes mod prime */
static void poly_rem(const poly *a, const poly *b, uint64_t mod, poly *rem)
{
    poly r = *a;
    int db = poly_degree(b);
    if (db == -1) { *rem = r; return; } /* division by zero */

    int da = poly_degree(&r);
    while (da >= db && da >= 0) {
        uint64_t inv = mod_inv(b->c[db], mod);
        uint64_t factor = (r.c[da] * inv) % mod;
        int shift = da - db;
        for (int i = 0; i <= db; i++) {
            int idx = i + shift;
            r.c[idx] = (r.c[idx] + mod - factor * b->c[i] % mod) % mod;
        }
        da = poly_degree(&r);
    }
    *rem = r;
}

/* gcd of two polynomials over F_p */
static poly poly_gcd(poly a, poly b, uint64_t mod)
{
    poly rem;
    while (poly_degree(&b) != -1) {
        poly_rem(&a, &b, mod, &rem);
        a = b;
        b = rem;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* obtain the n-th prime (1‑based) */
    seq_t prime_val = get_prime((uint64_t)n);
    uint64_t p = (uint64_t)prime_val;

    /* polynomial f(x)=x^3 - x^2 - x - 1 over F_p */
    poly f = {{p-1, p-1, p-1, 1}};   /* -1 ≡ p-1 mod p */

    /* compute x^p mod f */
    poly xp = pow_x_mod_f(p, p);

    /* compute g = x^p - x */
    poly x_poly = {{0,1,0,0}};
    poly g = poly_sub(&xp, &x_poly, p);

    /* gcd(f, g) */
    poly d = poly_gcd(f, g, p);
    int deg = poly_degree(&d);

    if (deg <= 0) return (seq_t)0;   /* zero polynomial or constant 1 */
    return (seq_t)deg;                /* degree 1 → 1 root, 2 → 2, 3 → 3 */
}
