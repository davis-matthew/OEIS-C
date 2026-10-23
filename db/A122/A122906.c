/*
 * A122906  Numbers m such that in Z/mZ the number of squares is less than or
 *          equal to the number of invertible elements.
 * https://oeis.org/A122906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that the number of distinct quadratic residues *
 *            modulo m is less than or equal to the number of units modulo m,
 *            i.e. * s(m) <= φ(m). Here s(m) is the number of residues y with
 *            a²≡y (mod m)
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq_2_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sq_2_power.h"

/* ------------------------------------------------------------------ */
/*  helper: power of an unsigned integer, safe for small exponents    */
/* ------------------------------------------------------------------ */
static seq_t pow_uint64(uint64_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)base;
    return res;
}

/* ------------------------------------------------------------------ */
/*  squares modulo 2^e                                               */

/* ------------------------------------------------------------------ */
/*  squares modulo p^e  (p odd prime)                                 */
/* ------------------------------------------------------------------ */
static seq_t sq_odd_prime_power(uint64_t p, idx_t e)
{
    /* s(p^e) = p^(e-1)*(p-1)/2 + 1 */
    seq_t p_pow = pow_uint64(p, e - 1);
    seq_t part = p_pow * ((seq_t)(p - 1) / 2);
    return part + (seq_t)1;
}

/* ------------------------------------------------------------------ */
/*  check whether m satisfies the defining inequality                  */
/* ------------------------------------------------------------------ */
static bool qualifies(uint64_t m)
{
    if (m == 0)
        return false;

    seq_t sq   = (seq_t)1;   /* start with {0} as one residue */
    seq_t phi  = (seq_t)1;   /* φ(1) = 1 */

    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }
    if (e2 > 0) {
        phi *= ((seq_t)1 << (e2 - 1));   /* φ(2^e2) = 2^(e2-1) */
        sq  *= sq_2_power(e2);
    }

    /* factor odd part */
    uint64_t n = m;
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                ++exp;
                n /= p;
            }
            seq_t p_pow = pow_uint64(p, exp - 1);
            phi *= p_pow * (seq_t)(p - 1);
            sq  *= sq_odd_prime_power(p, exp);
        }
    }
    if (n > 1) {               /* n itself is an odd prime */
        seq_t p_pow = (seq_t)1; /* exp = 1 */
        phi *= (seq_t)(n - 1);
        sq  *= sq_odd_prime_power(n, 1);
    }

    return sq <= phi;
}

/* ------------------------------------------------------------------ */
/*  public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_m = 1;          /* next integer to test */

    while (cache_len < (size_t)n) {
        if (qualifies(next_m)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }
    return cache[n - 1];   /* n is 1‑based */
}
