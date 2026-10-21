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
 * Range:     u8 n <= 27, u16 n <= 1156, u32 n <= 9043, u64 n <= 9043,
 *            u128 n <= 9043, big any n
 * Helpers:   bitlen_seq_t, isPrimeGeneric, poly_mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t);   */
#include "../utils/bitlen_seq_t.h"    /* unsigned bitlen_seq_t(seq_t); */
#include "../utils/poly_mul_mod.h"

/* --------------------------------------------------------------------- */
/* Polynomial arithmetic over GF(2)                                     */
/* --------------------------------------------------------------------- */

/* Reduce a modulo b (b ≠ 0).  Both a and b are treated as binary
 * polynomials: A polynomial multiplication is a XOR; a division is
 * performed by right shifting until the degree of a is smaller than
 * the degree of b. */
static inline seq_t poly_mod(seq_t a, seq_t b)
{
    unsigned deg_b = bitlen_seq_t(b) - 1;          /* degree of b */

    while (a != 0) {
        unsigned deg_a = bitlen_seq_t(a) - 1;
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
    unsigned deg = bitlen_seq_t(poly) - 1;     /* degree of the polynomial */

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

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
