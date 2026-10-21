/*
 * A091250  List of primitive irreducible polynomials over GF(2), interpreted
 *          as binary vectors, then written in base 10.
 * https://oeis.org/A091250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1110, u32 n <= 1110, u64 n <= 1110,
 *            u128 n <= 1110, big any n
 * Helpers:   factor_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091250: primitive irreducible polynomials over GF(2),
   lexicographically ordered, evaluated at X=2. 
   The function generates all primitive irreducible polynomials
   of increasing degree, stores them in a cache, and returns
   the (n‑1)-th element. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_uint32.h"

/* --- Bit‑degree of a polynomial represented as uint32_t --- */
static inline int poly_deg(uint32_t p) { return p ? 31 - __builtin_clz(p) : -1; }

/* --- Polynomial multiplication over GF(2) with modular reduction --- */
static inline uint32_t poly_mul_mod(uint32_t a, uint32_t b, uint32_t mod) {
    uint64_t res = 0, A = a, B = b;
    while (B) { if (B & 1) res ^= A; A <<= 1; B >>= 1; }
    int mod_deg = poly_deg(mod);
    while (res) {
        int res_deg = 63 - __builtin_clzll(res);
        if (res_deg < mod_deg) break;
        res ^= (uint64_t)mod << (res_deg - mod_deg);
    }
    return (uint32_t)res;
}

/* --- Polynomial modulus: a mod mod (both < 2^32) --- */
static inline uint32_t poly_mod(uint32_t a, uint32_t mod) {
    int mod_deg = poly_deg(mod);
    while (a) {
        int a_deg = poly_deg(a);
        if (a_deg < mod_deg) break;
        a ^= mod << (a_deg - mod_deg);
    }
    return a;
}

/* --- GCD of two polynomials over GF(2) --- */
static uint32_t gcd_poly(uint32_t a, uint32_t b) {
    while (b) { uint32_t r = poly_mod(a,b); a = b; b = r; }
    return a;
}

/* --- Modular exponentiation for polynomials over GF(2) --- */
static uint32_t poly_pow_mod(uint32_t base, uint32_t mod, uint32_t exp) {
    uint32_t result = 1u;
    while (exp) {
        if (exp & 1u) result = poly_mul_mod(result, base, mod);
        base = poly_mul_mod(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

/* --- Test whether a polynomial of degree 'deg' is irreducible --- */
static bool is_irreducible(uint32_t f, int deg) {
    if (deg == 1) return true;            /* x+1 is the only irreducible of degree 1 */

    uint32_t pf[5]; size_t pcnt;
    factor_uint32((uint32_t)deg, pf, &pcnt);

    for (size_t i = 0; i < pcnt; ++i) {
        int r = (int)pf[i];
        uint32_t exp = 1u << (deg / r);
        uint32_t g = poly_pow_mod(0x2u, f, exp);
        if (g == 1u) return false;       /* reducible */
    }
    uint32_t g = poly_pow_mod(0x2u, f, 1u << deg);
    if ((g ^ 0x2u) != 0u) return false;   /* not irreducible */
    return true;
}

/* --- Test whether a polynomial of degree 'deg' is primitive --- */
static bool is_primitive(uint32_t f, int deg, const uint32_t *prim_factors, size_t pcnt) {
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = prim_factors[i];
        uint64_t exp = ((uint64_t)1 << deg) - 1u; /* 2^deg - 1 */
        exp /= p;                                 /* (2^deg - 1)/p */
        uint32_t g = poly_pow_mod(0x2u, f, (uint32_t)exp);
        if (g == 1u) return false;               /* not primitive */
    }
    return true;
}

/* --- Cache for computed values ------------------------------------- */
static seq_t *cache      = NULL;    /* array of results */
static size_t cache_size = 0;       /* number of stored terms   */
static size_t cache_cap  = 0;       /* allocated capacity       */
static int    next_deg   = 1;       /* next degree to generate  */

/* --- Generate primitive irreducibles of degree 'deg' and append to cache --- */
static void generate_degree(int deg) {
    uint32_t min = ((uint32_t)1 << deg) | 1u;            /* constant term 1 */
    uint32_t max = ((uint32_t)1 << (deg + 1));           /* exclusive */

    uint32_t prim_factors[32]; size_t pcnt;
    factor_uint32(((uint32_t)1 << deg) - 1u, prim_factors, &pcnt);

    for (uint32_t f = min; f < max; f += 2u) {
        if (!is_irreducible(f, deg)) continue;
        if (!is_primitive(f, deg, prim_factors, pcnt)) continue;

        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)f;
    }
}

/* --- Ensure the cache contains at least n terms --- */
static void ensure_cache_size(idx_t n) {
    while (cache_size < (size_t)n) {
        generate_degree(next_deg++);
    }
}

/* --- Public sequence function ------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_cache_size(n);
    return cache[n - 1];
}
