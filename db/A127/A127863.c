/*
 * A127863  Coefficients of modular form for elliptic curve "243b1": y^2 + y =
 *          x^3 + 2 divided by q in powers of q^3.
 * https://oeis.org/A127863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(3*n + 1) * where b is the multiplicative function
 *            defined in A127863: * * b(3^e) = 0 for any e ≥ 1 * * for a prime
 *            p ≡ 2 (mod 3): * b(p^e) = 0 if e is odd * b(p^e) = (-p)^(e/2) if
 *            e is even * * for a prime p ≡ 1 (mod 3): * b(p^e) =
 *            b(p^e-1)*b(p) - p*b(p^e-2) (e ≥ 2) * with b(p^1) = b(p)...
 * Range:     i8 n <= 309, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t a, uint64_t n); */

/* small cache for b(p) where p ≡ 1 (mod 3) */
#define B_CACHE_SIZE 8192
struct BEntry { uint32_t p; seq_t val; };
static struct BEntry bcache[B_CACHE_SIZE];
static size_t bcache_used = 0;

/* helper: compute b(p) for prime p ≡ 1 (mod 3) */
static seq_t compute_bprime(uint32_t p)
{
    /* look in cache */
    for (size_t i = 0; i < bcache_used; ++i)
        if (bcache[i].p == p)
            return bcache[i].val;

    /* compute the sum of Jacobi symbols */
    seq_t sum = 0;
    for (uint32_t x = 0; x < p; ++x) {
        /* t = (4*x^3 + 9) mod p */
        uint64_t xx = (uint64_t)x * x % p;          /* x^2 mod p */
        xx = (uint64_t)xx * x % p;                 /* x^3 mod p */
        uint64_t t = (4ULL * xx) % p;
        t = (t + 9) % p;
        int64_t js = jacobi_symbol(t, p);          /* js ∈ { -1, 0, 1 } */
        sum += (seq_t)js;
    }
    sum = -sum;

    /* store in cache */
    if (bcache_used < B_CACHE_SIZE) {
        bcache[bcache_used].p   = p;
        bcache[bcache_used].val = sum;
        ++bcache_used;
    }
    return sum;
}

/* helper: multiplicative b(m) for m ≥ 1 */
static seq_t compute_b(idx_t m)
{
    if (m == 1)
        return 1;

    idx_t n = m;
    seq_t res = 1;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p != 0)
            continue;
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }

        seq_t val = 0;

        if (p == 3) {
            val = 0;                    /* any power of 3 gives zero */
        } else if (p % 3 == 2) {
            if (e & 1)
                val = 0;                /* odd exponent → 0 */
            else {
                seq_t cur = 1;
                seq_t base = -((seq_t)p);
                for (int k = 0; k < e / 2; ++k)
                    cur *= base;
                val = cur;
            }
        } else /* p % 3 == 1 */ {
            seq_t bp = compute_bprime((uint32_t)p);
            if (e == 1) {
                val = bp;
            } else {
                seq_t prev2 = 1;   /* b(p^0) */
                seq_t prev1 = bp;  /* b(p^1) */
                seq_t cur   = 0;
                for (int k = 2; k <= e; ++k) {
                    cur = prev1 * prev1 - ((seq_t)p) * prev2;
                    prev2 = prev1;
                    prev1 = cur;
                }
                val = prev1;
            }
        }

        res *= val;
        if (res == 0)
            return 0;
    }

    /* remaining prime factor > sqrt(m) */
    if (n > 1) {
        uint32_t p = (uint32_t)n;
        seq_t val = 0;
        if (p == 3) {
            val = 0;
        } else if (p % 3 == 2) {
            val = 0;                    /* exponent 1 is odd → 0 */
        } else {
            val = compute_bprime(p);
        }
        res *= val;
        if (res == 0)
            return 0;
    }

    return res;
}

seq_t a(idx_t n)
{
    /* a(n) = b(3*n + 1) */
    idx_t m = 3 * n + 1;
    return compute_b(m);
}
