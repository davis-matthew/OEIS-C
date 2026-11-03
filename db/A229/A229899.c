/*
 * A229899  a(n) = |{0<g<p_n: g-1, g, g+1 are primitive roots mod p_n}|, where
 *          p_n denotes the n-th prime.
 * https://oeis.org/A229899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Validated: standard, pass, 116 terms through n = 368 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A229899
 *
 *  Definition:  a(n) = |{ 0 < g < pₙ :   g-1, g, g+1 are primitive
 *                                 roots modulo the n‑th prime pₙ }|
 *
 *  The function computes all primitive roots of the n‑th prime by
 *  testing each candidate g with the factorisation of pₙ−1, counts the
 *  number of admissible triples, and memoises the result.
 *
 *  Dependencies:  get_prime, mod_pow (both from the utils directory).
 *
 *  Complexity:  O(pₙ log pₙ) per new prime; pₙ ≈ n log n, so this
 *  stays well below 2 s for n ≤ 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* --- Memoisation structures ------------------------------------------- */
static seq_t *cache = NULL;        /* a[n-1] stored */
static bool  *used  = NULL;        /* whether a[n-1] is ready */
static size_t cache_cap = 0;

/* --- Helper: factor (p-1) into distinct primes ------------------------ */
static void factor_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t f = 2; f * f <= n; ++f)
        if (n % f == 0) {
            factors[(*cnt)++] = f;
            while (n % f == 0) n /= f;
        }
    if (n > 1) factors[(*cnt)++] = n;
}

/* --- Count primitive‑root triples for a single prime ----------------- */
static seq_t count_triples(uint64_t p)
{
    /* p is always odd prime ≥ 7 here */
    uint64_t m = p - 1;
    uint64_t facs[32];
    size_t  nf;
    factor_distinct(m, facs, &nf);

    /* Allocate array of flags for primitive roots   */
    uint8_t *is_prim = (uint8_t*)calloc((size_t)p, 1);
    if (!is_prim) exit(EXIT_FAILURE);

    /* Test each candidate g (2 … p−1) for primitivity */
    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < nf; ++i) {
            if (powmod((seq_t)g, m / facs[i], (seq_t)p) == (seq_t)1) {
                ok = false;
                break;
            }
        }
        if (ok) is_prim[(size_t)g] = 1;
    }

    /* Count triples g−1, g, g+1 of primitive roots                 */
    seq_t cnt = 0;
    for (uint64_t g = 2; g <= p - 2; ++g)
        if (is_prim[(size_t)g] &&
            is_prim[(size_t)(g - 1)] &&
            is_prim[(size_t)(g + 1)])
            ++cnt;

    free(is_prim);
    return cnt;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* resize memoisation containers if needed                        */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        bool *tb = (bool*)realloc(used, new_cap * sizeof(bool));
        if (!tb) exit(EXIT_FAILURE);
        used = tb;
        for (size_t i = cache_cap; i < new_cap; ++i) used[i] = false;
        cache_cap = new_cap;
    }

    if (!used[n - 1]) {
        uint32_t p = get_prime(n);           /* n‑th prime (odd, ≥ 7 for n≥5) */
        if (p <= 5)       /* p ∈ {2,3,5} → a(n)=0 by definition  */
            cache[n - 1] = (seq_t)0;
        else
            cache[n - 1] = count_triples((uint64_t)p);
        used[n - 1] = true;
    }
    return cache[n - 1];
}
