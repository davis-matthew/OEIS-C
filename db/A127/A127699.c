/*
 * A127699  Length of period of the sequence (1^1^1^..., 2^2^2^..., 3^3^3^...,
 *          4^4^4^..., ...) modulo n.
 * https://oeis.org/A127699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(n, a(lambda(n))), where lambda is Carmichael's
 *            reduced totient function
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 718, u64 n <= 8636,
 *            u128 n <= 9899, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"

/* --- Helper: greatest common divisor for seq_t --------------------------------- */

/* --- Helper: integer square root of uint64_t --------------------------------- */
static idx_t
isqrt_uint64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << (63 - __builtin_clzll(x)); /* highest power of two <= x */
    while (bit) {
        if (r + bit <= x) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* --- Helper: pow_seq (seq_t base, unsigned exp) -------------------------------- */
static seq_t
pow_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

/* --- Carmichael's lambda for uint64_t ------------------------------------- */
static seq_t
lambda_uint64(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t lcm_val = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;
        unsigned e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        seq_t lam;
        if (p == 2) {
            if (e == 1)
                lam = 1;             /* λ(2)  = 1  */
            else if (e == 2)
                lam = 2;             /* λ(4)  = 2  */
            else
                lam = pow_seq((seq_t)2, e - 2); /* λ(2^e)=2^{e-2}  */
        } else {
            /* λ(p^e) = (p-1)*p^{e-1} for odd prime p */
            lam = (seq_t)(p - 1) * pow_seq((seq_t)p, e - 1);
        }
        seq_t g = gcd_seq(lcm_val, lam);
        lcm_val = lcm_val / g * lam;
    }
    if (temp > 1) {               /* remaining prime factor */
        seq_t lam = (temp == 2) ? 1 : (seq_t)(temp - 1);
        seq_t g = gcd_seq(lcm_val, lam);
        lcm_val = lcm_val / g * lam;
    }
    return lcm_val;
}

/* --- Sequence cache ----------------------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure cache large enough for index n */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (n + 1 > 16 ? n + 1 : 16);
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
    } else if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) {          /* allocation failure – should not happen */
            exit(1);
        }
        /* zero new part */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* --- Main sequence function -------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)         /* not in domain; return 0 */
        return 0;

    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];

    seq_t result;
    if (n == 1) {
        result = 1;
    } else {
        seq_t lam = lambda_uint64(n);                /* λ(n) fits in idx_t for n>1 */
        idx_t lam_idx = (idx_t)lam;
        seq_t period = a(lam_idx);                  /* recursive step */
        seq_t g = gcd_seq((seq_t)n, period);
        result = (seq_t)n / g * period;              /* lcm(n, period) */
    }
    cache[n] = result;
    return result;
}
