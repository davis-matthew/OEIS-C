/*
 * A067756  Prime hypotenuses of Pythagorean triangles with a prime leg.
 * https://oeis.org/A067756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 807, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A067756 – prime hypotenuses of Pythagorean triangles with a prime leg.  
   Formula used in the code:  
   a(n) = (q^2 + 1) / 2, where q is a prime and (q^2 + 1)/2 is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

#ifdef __cplusplus
#include <vector>
#endif

/* ----------  cache  ---------- */
#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* unrecoverable memory allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* ----------  next prime q  ---------- */
static uint32_t next_q = 3;   /* next odd candidate to be tested for primality */

/* ----------  generate terms up to n (1‑based) ---------- */
static void generate_terms(idx_t n)
{
#ifdef __cplusplus
    while (cache.size() < (size_t)n) {
        uint32_t q = next_q;
        for (;;) {
            if (isPrimeGeneric((seq_t)q))
                break;
            q += 2;                   /* skip even numbers */
        }
        next_q = q + 2;                /* prepare the next candidate */

        seq_t q_t = (seq_t)q;
        seq_t p = (q_t * q_t + 1) / 2;
        if (isPrimeGeneric(p))
            cache.push_back(p);
    }
#else
    while (cache_len < (size_t)n) {
        uint32_t q = next_q;
        for (;;) {
            if (isPrimeGeneric((seq_t)q))
                break;
            q += 2;
        }
        next_q = q + 2;

        seq_t q_t = (seq_t)q;
        seq_t p = (q_t * q_t + 1) / 2;
        if (isPrimeGeneric(p)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = p;
        }
    }
#endif
}

/* ----------  public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* offset convention – not used for OEIS a(1)... */
    generate_terms(n);
#ifdef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
