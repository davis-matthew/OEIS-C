/*
 * A206496  The Connes-Moscovici weight of the rooted tree with Matula-Goebel
 *          number n. It is defined as the number of ways to build up the
 *          rooted tree from the one-vertex tree by adding successively edges
 *          to the existing vertices.
 * https://oeis.org/A206496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = V(n)! / (TF(n)*SF(n)) where V(n) is the number of
 *            vertices of the rooted tree with Matula-Goebel number n, TF(n)
 *            is the tree factorial, SF(n) is the symmetry factor. The
 *            function below memoises the values of V, TF and SF and then
 *            returns the result
 * Range:     u8 n <= 14, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_primes_upto, factorial, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factorial.h"          /* factorial(idx_t) -> seq_t */
#include "../utils/build_primes_upto.h"  /* build_primes_upto(limit, &primes, &cnt) */
#include "../utils/seq_alloc.h"          /* seq_alloc(size_t) -> seq_t* */
#include "../utils/seq_realloc.h"       /* seq_realloc(seq_t*,size_t,size_t) -> seq_t* */

/* cache for V, TF and SF ------------------------------------------- */
static seq_t *V_cache   = NULL;
static seq_t *TF_cache  = NULL;
static seq_t *SF_cache  = NULL;
static bool   *valid    = NULL;
static size_t  cache_cap = 0;

/* ensure that the cache arrays have room for indices up to n ----------------- */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;          /* already large enough */

    size_t new_cap = (size_t)(n + 1) * 2;   /* grow geometrically */

    if (cache_cap == 0) {
        V_cache  = seq_alloc(new_cap);
        TF_cache = seq_alloc(new_cap);
        SF_cache = seq_alloc(new_cap);
    } else {
        V_cache  = seq_realloc(V_cache,  cache_cap, new_cap);
        TF_cache = seq_realloc(TF_cache, cache_cap, new_cap);
        SF_cache = seq_realloc(SF_cache, cache_cap, new_cap);
    }

    bool *tmp = (bool *)realloc(valid, new_cap * sizeof(bool));
    valid = tmp;
    for (size_t i = cache_cap; i < new_cap; ++i) valid[i] = false;

    cache_cap = new_cap;
}

/* prime list -------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;
static idx_t    prime_limit   = 0;

static void ensure_primes(idx_t n)
{
    if (prime_limit >= n) return;   /* already sufficient */

    if (prime_list) free(prime_list);
    build_primes_upto(n, &prime_list, &prime_cnt);
    prime_limit = n;
}

/* recursive computation of V, TF and SF --------------------------------- */
static void compute_tree(idx_t n, seq_t *pV, seq_t *pTF, seq_t *pSF)
{
    if (n == 1) {                     /* one‑vertex tree */
        *pV = 1; *pTF = 1; *pSF = 1;
        return;
    }

    ensure_cache(n);
    if (valid[n]) {                   /* already computed */
        *pV  = V_cache[n];
        *pTF = TF_cache[n];
        *pSF = SF_cache[n];
        return;
    }

    ensure_primes(n);

    idx_t temp = n;
    seq_t subV    = 0;                 /* Σ e_i * V(child) */
    seq_t prodTF  = 1;                 /* Π TF(child)^e_i */
    seq_t prodSF  = 1;                 /* Π SF(child)^e_i * e_i! */

    while (temp > 1) {
        idx_t prime = 0;
        idx_t exp   = 0;

        /* find first prime divisor of temp */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > temp) break;
            if (temp % p == 0) {
                prime = p;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                break;
            }
        }
        if (prime == 0) {              /* temp itself is prime */
            prime = temp;
            exp   = 1;
            temp  = 1;
        }

        /* index of this prime (1‑based) */
        size_t idx = 0;
        while (idx < prime_cnt && prime_list[idx] != prime) ++idx;
        idx_t child = (idx + 1);       /* matula number of the child subtree */

        /* recurse to get data for the child subtree */
        seq_t Vc, TFc, SFc;
        compute_tree(child, &Vc, &TFc, &SFc);

        subV += (seq_t)exp * Vc;

        /* multiply TFc^exp */
        seq_t powTF = 1;
        for (idx_t i = 1; i <= exp; ++i) powTF *= TFc;
        prodTF *= powTF;

        /* multiply SFc^exp */
        seq_t powSF = 1;
        for (idx_t i = 1; i <= exp; ++i) powSF *= SFc;
        prodSF *= powSF;

        /* multiply by exp! (symmetry factor) */
        seq_t fact = 1;
        for (idx_t i = 2; i <= exp; ++i) fact *= (seq_t)i;
        prodSF *= fact;
    }

    seq_t V   = subV + 1;               /* plus the root vertex        */
    seq_t TF  = V * prodTF;
    seq_t SF  = prodSF;

    /* store in cache */
    V_cache[n]  = V;
    TF_cache[n] = TF;
    SF_cache[n] = SF;
    valid[n]    = true;

    *pV  = V;
    *pTF = TF;
    *pSF = SF;
}

/* public sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    seq_t V, TF, SF;
    compute_tree(n, &V, &TF, &SF);

    seq_t numerator   = factorial((idx_t)V);
    seq_t denominator = TF * SF;
    return numerator / denominator;
}
