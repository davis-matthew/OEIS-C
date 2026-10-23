/*
 * A123475  Product of the primitive roots of prime(n).
 * https://oeis.org/A123475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primitive roots modulo the n‑th prime
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   factorize, get_prime, mod_pow, seq_mul, seq_one, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/get_prime.h"       /* uint32_t get_prime(idx_t k); */
#include "../utils/factorize.h"      /* size_t factorize(uint64_t n, uint64_t primes[], int exps[]); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/seq_mul.h"        /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/seq_one.h"        /* seq_t seq_one(void); */
#include "../utils/u64_gcd.h"        /* uint64_t u64_gcd(uint64_t a, uint64_t b); */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t  *cache     = NULL;
    static bool   *computed  = NULL;
    static idx_t   cache_len = 0;

    if (n == 0)        /* unused domain – defensive */
        return seq_one();

    /* grow cache if required */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache;
        bool  *new_comp;

        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        new_comp  = (bool *)realloc(computed, new_len * sizeof(bool));
        if (!new_cache || !new_comp) {
            free(cache);
            free(computed);
            abort();
        }

        /* initialise new "computed" entries to false */
        memset(new_comp + cache_len, 0, (new_len - cache_len) * sizeof(bool));

        cache    = new_cache;
        computed = new_comp;
        cache_len = new_len;
    }

    /* return memoised value if available */
    if (computed[n])
        return cache[n];

    seq_t result;

    /* special cases */
    if (n == 1) {                     /* a(1) = 1 */
        result = seq_one();
    } else if (n == 2) {               /* a(2) = 2 */
        result = (seq_t)2;
    } else {
        /* the n-th prime */
        uint32_t p32 = get_prime((idx_t)n);
        seq_t    p   = (seq_t)p32;          /* prime as seq_t */

        /* factorisations of p‑1 (Euler phi) */
        uint64_t pf[64];
        int      exp[64];
        size_t   nf = factorize((uint64_t)p32 - 1, pf, exp);
        uint64_t m  = (uint64_t)p32 - 1;    /* φ(p) = p‑1 */

        /* find a primitive root g of the prime p */
        seq_t g = 0;
        for (seq_t cand = 2; cand < p; ++cand) {
            bool ok = true;
            for (size_t i = 0; i < nf; ++i) {
                uint64_t q = pf[i];
                if (mod_pow(cand, m / q, p) == ((seq_t)1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) { g = cand; break; }
        }
        if (g == 0)  /* safety, should not happen for p>2 */
            g = 2;

        /* product over all primitive roots: r = g^k (mod p) for k coprime to m */
        seq_t prod = seq_one();
        for (uint64_t k = 1; k < m; ++k) {
            if (u64_gcd(k, m) == 1) {
                seq_t r = mod_pow(g, k, p);
                prod = seq_mul(prod, r);
            }
        }
        result = prod;
    }

    /* store computed value */
    cache[n]    = result;
    computed[n] = true;
    return result;
}
