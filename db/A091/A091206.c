/*
 * A091206  Primes whose binary representation encodes a polynomial
 *          irreducible over GF(2).
 * https://oeis.org/A091206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p whose binary representation encodes an *
 *            irreducible polynomial over GF(2). The function finds primes in
 *            * increasing order and tests each for irreducibility using
 *            Rabin’s * test over GF(2). Results are cached for efficient
 *            repeated calls
 * Range:     u8 n <= 27, u16 n <= 1156, u32 n <= 10226, u64 n <= 10226,
 *            u128 n <= 10226, big any n
 * Validated: standard, pass, 71 terms through n = 9043 (2026-11-03)
 * Helpers:   bit_length, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include <limits.h>
#include "../utils/bit_length.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Multiply a and b modulo a monic polynomial mod of degree moddeg. */
static inline seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod, unsigned moddeg)
{
    seq_t res = 0;
    seq_t aa = a, bb = b;

    while (bb) {
        if (bb & 1)
            res ^= aa;                     /* addition over GF(2) */

        bb >>= 1;
        aa <<= 1;

        /* If aa and therefore the product grows to deg+1, reduce it. */
        if (aa & ((seq_t)1 << moddeg))
            aa ^= mod;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Polynomial arithmetic over GF(2)                                     */
/* --------------------------------------------------------------------- */

/* Reduce a modulo b (b ≠ 0).  Both a and b are treated as binary
 * polynomials: A polynomial multiplication is a XOR; a division is
 * performed by right shifting until the degree of a is smaller than
 * the degree of b. */
static inline seq_t poly_mod(seq_t a, seq_t b)
{
    unsigned deg_b = bit_length(b) - 1;          /* degree of b */

    while (a != 0) {
        unsigned deg_a = bit_length(a) - 1;
        if (deg_a < deg_b)
            break;
        a ^= b << (deg_a - deg_b);                /* subtract b * x^{deg_a-deg_b} */
    }
    return a;                                      /* remainder of a / b */
}

/* GCD of two polynomials over GF(2). */
static inline seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* --------------------------------------------------------------------- */
/* Irreducibility test over GF(2)                                      */
/* --------------------------------------------------------------------- */

/* Returns true iff a monic polynomial encoded by poly is irreducible
 * over GF(2).  The routine implements Rabin’s test: for each i
 * (1 ≤ i ≤ floor(d/2)) check gcd(x^(2^i) − x,poly) = 1 and finally
 * confirm that x^(2^d) ≡ x (mod poly). */
static bool poly_irreducible(seq_t poly)
{
    unsigned deg = bit_length(poly) - 1;     /* degree of the polynomial */

    if (deg <= 1)                /* X and X+1 are always irreducible  */
        return true;

    seq_t x = 2;                 /* the polynomial X  */
    seq_t t = x;

    for (unsigned i = 1; 2 * i <= deg; ++i) {
        t = poly_mul_mod(t, t, poly, deg);    /* t = t^2 mod poly */
        seq_t g = poly_gcd(t ^ x, poly);      /* t^(-)x */
        if (g != 1)
            return false;                      /* reducible */
    }

    /* Final check: X^(2^deg) ≡ X (mod poly). */
    t = x;
    for (unsigned i = 0; i < deg; ++i)
        t = poly_mul_mod(t, t, poly, deg);

    return t == x;
}

/* --------------------------------------------------------------------- */
/* Cache of computed terms                                            */
/* --------------------------------------------------------------------- */

static seq_t *cache      = NULL;   /* array of already found terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity                */

/* Grow the cache to hold at least new_len elements.  The
 * cast to (seq_t *) is required for C++ compilation when
 * seq_t is an arbitrary precision class. */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap < new_len)
        new_cap = new_len;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = 2;  /* first term */
        else
            candidate = cache[cache_len - 1] + 1;

        /* After the first term, only odd candidates are possible. */
        if (candidate > 2 && (candidate & 1) == 0)
            ++candidate;

        for (;;) {
            if (isPrimeGeneric(candidate) && poly_irreducible(candidate)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = candidate;
                break;
            }
            candidate += (candidate == 2 ? 1 : 2);   /* skip even numbers */
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public function                                                    */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n ≤ 0 */

    ensure_cache(n);
    return cache[n - 1];
}
