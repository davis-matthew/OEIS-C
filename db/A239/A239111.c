/*
 * A239111  Smallest Pell number (see A000129) divisible by n-th prime.
 * https://oeis.org/A239111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 18, u128 n <= 27,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol, mul_mod64, pell_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A239111: the smallest Pell number divisible by the n‑th prime. */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/mul_mod64.h"
#include "../utils/pell_exact.h"

/* -------------------------------------------------------------------------- */
/*  Helper: matrix exponentiation of the Pell matrix, returning P(n).          */

/* -------------------------------------------------------------------------- */
/*  Helper: fast doubling modulo p, returning P(n) mod p.                     */
/* -------------------------------------------------------------------------- */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return mul_mod64(a, b, mod);
}

static uint64_t pell_mod_uint64(idx_t n, uint64_t mod)
{
    struct Mat { uint64_t a, b, c, d; };
    struct Mat res = { 1ULL, 0ULL, 0ULL, 1ULL };
    struct Mat base = { 2ULL % mod, 1ULL % mod, 1ULL % mod, 0ULL };

    while (n) {
        if (n & 1) {
            uint64_t na = (mul_mod_u64(res.a, base.a, mod) +
                           mul_mod_u64(res.b, base.c, mod)) % mod;
            uint64_t nb = (mul_mod_u64(res.a, base.b, mod) +
                           mul_mod_u64(res.b, base.d, mod)) % mod;
            uint64_t nc = (mul_mod_u64(res.c, base.a, mod) +
                           mul_mod_u64(res.d, base.c, mod)) % mod;
            uint64_t nd = (mul_mod_u64(res.c, base.b, mod) +
                           mul_mod_u64(res.d, base.d, mod)) % mod;
            res.a = na; res.b = nb; res.c = nc; res.d = nd;
        }

        uint64_t na = (mul_mod_u64(base.a, base.a, mod) +
                       mul_mod_u64(base.b, base.c, mod)) % mod;
        uint64_t nb = (mul_mod_u64(base.a, base.b, mod) +
                       mul_mod_u64(base.b, base.d, mod)) % mod;
        uint64_t nc = (mul_mod_u64(base.c, base.a, mod) +
                       mul_mod_u64(base.d, base.c, mod)) % mod;
        uint64_t nd = (mul_mod_u64(base.c, base.b, mod) +
                       mul_mod_u64(base.d, base.d, mod)) % mod;
        base.a = na; base.b = nb; base.c = nc; base.d = nd;

        n >>= 1;
    }
    return res.b;          /* P(n) mod p */
}

/* -------------------------------------------------------------------------- */
/*  Comparator for qsort on idx_t values.                                      */
/* -------------------------------------------------------------------------- */
static int cmp_idx(const void *a, const void *b)
{
    unsigned long long aa = *(const unsigned long long *)a;
    unsigned long long bb = *(const unsigned long long *)b;
    return (aa > bb) - (aa < bb);
}

/* -------------------------------------------------------------------------- */
/*  Main function: a(n) = smallest Pell number divisible by the n‑th prime.   */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑indexed). */
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;

    /* Special case for prime 2. */
    if (p == 2ULL)
        return pell_exact( (idx_t)2 );

    /* Compute the Legendre symbol (2|p). */
    int leg = jacobi_symbol((uint64_t)2, p);
    uint64_t t = p - (uint64_t)leg;          /* t = p – (2|p) */

    /* Enumerate all divisors of t. */
    const size_t MAX_DIV = 256;
    idx_t divs[MAX_DIV];
    size_t divcnt = 0;

    for (idx_t i = 1; i * i <= t; ++i) {
        if (t % i == 0) {
            if (divcnt < MAX_DIV) divs[divcnt++] = i;
            idx_t e = t / i;
            if (i != e && divcnt < MAX_DIV) divs[divcnt++] = e;
        }
    }

    /* Sort divisors ascending. */
    qsort(divs, divcnt, sizeof(idx_t), cmp_idx);

    /* Find the smallest divisor k such that P(k) ≡ 0 (mod p). */
    idx_t k = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        if (pell_mod_uint64(divs[i], p) == 0ULL) {
            k = divs[i];
            break;
        }
    }
    if (k == 0)               /* fallback, should not happen */
        k = t;

    /* Return the exact Pell number P(k). */
    return pell_exact(k);
}
