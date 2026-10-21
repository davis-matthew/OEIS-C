/*
 * A238186  Primes with odd Hamming weight that as polynomials over GF(2) are
 *          reducible.
 * https://oeis.org/A238186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 1912, u64 n <= 9422,
 *            u128 n <= 9422, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p with odd Hamming weight and whose binary polynomial
 * over GF(2) is reducible.
 * The function generates terms by simple search.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

/* popcount for generic seq_t */
static inline unsigned popcount_seq(seq_t x)
{
    unsigned cnt = 0;
    while (x)
    {
        cnt += (unsigned)(x & (seq_t)1);
        x >>= 1;
    }
    return cnt;
}

/* polynomial long division (a mod b) over GF(2) */
static inline seq_t poly_mod(seq_t a, seq_t mod)
{
    if (mod == (seq_t)0)
        return a;
    idx_t deg_mod = bitlen_seq_t(mod) - 1;
    idx_t deg_a   = bitlen_seq_t(a) - 1;
    while (deg_a >= deg_mod && a != (seq_t)0)
    {
        idx_t shift = deg_a - deg_mod;
        a ^= mod << shift;
        deg_a = bitlen_seq_t(a) - 1;
    }
    return a;
}

/* polynomial GCD over GF(2) */
static inline seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* multiplication of two polynomials modulo mod */
static inline seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod)
{
    seq_t res = (seq_t)0;
    while (b != (seq_t)0)
    {
        if (b & (seq_t)1)
            res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return poly_mod(res, mod);
}

/* compute x^(2^t) mod f by t squarings, starting from x = 2 */
static inline seq_t poly_pow_x2_t(idx_t t, seq_t f)
{
    seq_t x = (seq_t)2;
    for (idx_t i = 0; i < t; ++i)
        x = poly_mul_mod(x, x, f);
    return x;
}

/* Rabin's test for irreducibility over GF(2) */
static bool is_irreducible_poly(seq_t f)
{
    if (f <= (seq_t)1)
        return false;
    idx_t deg = bitlen_seq_t(f) - 1; /* degree of the polynomial */
    if (deg <= 1)
        return true; /* degree 0 or 1 are irreducible */

    /* factor deg into distinct prime divisors */
    idx_t d = deg;
    idx_t factors[10];
    int fcnt = 0;
    for (idx_t p = 2; p * p <= d; ++p)
    {
        if (d % p == 0)
        {
            factors[fcnt++] = p;
            while (d % p == 0)
                d /= p;
        }
    }
    if (d > 1)
        factors[fcnt++] = d;

    /* test for each prime divisor */
    for (int i = 0; i < fcnt; ++i)
    {
        idx_t t = deg / factors[i];
        seq_t g = poly_pow_x2_t(t, f); /* x^(2^t) mod f */
        seq_t diff = g ^ (seq_t)2;     /* x^(2^t) - x */
        if (poly_gcd(diff, f) != (seq_t)1)
            return false; /* reducible */
    }

    /* final test: x^(2^deg) mod f should equal x (i.e., 2) */
    seq_t h = poly_pow_x2_t(deg, f);
    return (h == (seq_t)2);
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t candidate = 3;   /* start from the first odd prime >=3 */
    static idx_t found = 0;

    while (1)
    {
        if (is_prime_big(candidate))
        {
            if ( (popcount_seq(candidate) & 1) == 1  /* odd Hamming weight */
                 && !is_irreducible_poly(candidate)  /* reducible polynomial */
               )
            {
                ++found;
                if (found == n)
                {
                    /* advance candidate for the next call */
                    candidate += (seq_t)2;
                    return candidate - (seq_t)2;
                }
            }
        }
        candidate += (seq_t)2;  /* check next odd number */
    }
}
