/*
 * A147611  The 3rd Witt transform of A000027.
 * https://oeis.org/A147611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A147611 : the 3rd Witt transform of A000027
 *  Formula (recurrence derived from the generating function)
 *  Let a(0)…a(9) be the first ten terms shown in the OEIS entry.
 *  For n ≥ 10:
 *      a(n) =   4·a(n‑1) – 6·a(n‑2) + 6·a(n‑3) – 9·a(n‑4)
 *             +12·a(n‑5) – 9·a(n‑6) + 6·a(n‑7) – 6·a(n‑8)
 *             + 4·a(n‑9) –   a(n‑10)
 *
 *  This recurrence follows from the denominator
 *      (1−x)⁶(1+x+x²)²  = 1 −4x +6x² −6x³ +9x⁴ −12x⁵ +9x⁶ −6x⁷ +6x⁸ −4x⁹ +x¹⁰ .
 *
 *  The implementation keeps a cached array of computed values and
 *  adds new entries on demand with the recurrence above.  All arithmetic
 *  is performed in the type seq_t, which is whichever integer or bignum
 *  class is supplied by the build system.
 */

#include <stdlib.h>
#include <stdint.h>                    /* for uint64_t (idx_t) */

static const seq_t init[10] = {
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)2, (seq_t)7, (seq_t)18, (seq_t)42,
    (seq_t)84, (seq_t)153
};

static seq_t *cache = NULL;           /* cache of a(n) */
static idx_t cache_sz = 0;            /* number of entries stored */
static size_t cache_cap = 0;          /* allocated capacity */

/* ensure cache contains values up to n */
static void ensure_cache(idx_t n)
{
    if (cache_sz > n) return;

    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        cache_cap = needed;
    }

    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i < 10) {
            cache[i] = init[(size_t)i];
        } else {
            /* compute a(i) via the recurrence
             *  a =  (4*a_{i-1} + 6*a_{i-3} + 12*a_{i-5} + 6*a_{i-7} + 4*a_{i-9})
             *     - (6*a_{i-2} + 9*a_{i-4} + 9*a_{i-6} + 6*a_{i-8} + a_{i-10})        */
            seq_t pos = 0;
            pos += (seq_t)4 * cache[(size_t)(i - 1)];
            pos += (seq_t)6 * cache[(size_t)(i - 3)];
            pos += (seq_t)12 * cache[(size_t)(i - 5)];
            pos += (seq_t)6 * cache[(size_t)(i - 7)];
            pos += (seq_t)4 * cache[(size_t)(i - 9)];

            seq_t neg = 0;
            neg += (seq_t)6 * cache[(size_t)(i - 2)];
            neg += (seq_t)9 * cache[(size_t)(i - 4)];
            neg += (seq_t)9 * cache[(size_t)(i - 6)];
            neg += (seq_t)6 * cache[(size_t)(i - 8)];
            neg += cache[(size_t)(i - 10)];

            cache[i] = pos - neg;
        }
    }
    cache_sz = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
