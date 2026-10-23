/*
 * A169839  EKG sequence started at 4 instead of 2.
 * https://oeis.org/A169839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169839: EKG sequence started at 4 instead of 2
   a(1)=4, a(2)=2, thereafter each term is the smallest unused integer
   sharing a non‑trivial GCD with the previous term. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

/* ensure that a byte array is large enough to index up to idx */
static void ensure_used(unsigned char **used, size_t *size, size_t idx)
{
    if ((*used) &&(idx < *size))
        return;
    size_t new_size = *size ? *size : 1;
    while (new_size <= idx) new_size <<= 1;
    unsigned char *tmp = (unsigned char*)realloc(*used, new_size);
    if (!tmp) exit(1);                 /* allocation failure */
    /* zero‑initialize newly allocated part */
    if (*size < new_size)
        memset(tmp + *size, 0, new_size - *size);
    *used = tmp;
    *size = new_size;
}

/* ensure that a seq_t array is large enough to store count elements */
static void ensure_cache(seq_t **cache, size_t *size, size_t count)
{
    if ((*cache) && (count <= *size))
        return;
    size_t new_size = *size ? *size : 1;
    while (new_size < count) new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(*cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);
    *cache = tmp;
    *size = new_size;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* sequence cache (0‑based) */
    static size_t cache_size = 0;
    static size_t cache_len   = 0;       /* number of terms computed so far */

    static unsigned char *used = NULL;   /* table of used values (byte per value) */
    static size_t used_size = 0;

    if (n == 0)   /* undefined for n = 0 – behave as if n is 1 */
        return (seq_t)4;

    /* initialise structures on first call */
    if (!cache) {
        cache_size = 16;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
    }
    if (!used) {
        used_size = 1024;
        used = (unsigned char*)calloc(used_size, 1);
        if (!used) exit(1);
    }

    /* compute terms until we reach n */
    while (cache_len < n) {
        seq_t val;
        if (cache_len == 0) {
            val = (seq_t)4;
        } else if (cache_len == 1) {
            val = (seq_t)2;
        } else {
            /* find the smallest unused integer having gcd>1 with the previous term */
            seq_t prev = cache[cache_len - 1];
            /* factorise prev (assume it fits in uint64_t for factor extraction) */
            uint64_t tmp = (uint64_t)prev;
            uint64_t primes[20];
            size_t pcnt = 0;
            uint64_t f = 2;
            while (f * f <= tmp) {
                if (tmp % f == 0) {
                    primes[pcnt++] = f;
                    while (tmp % f == 0) tmp /= f;
                }
                f = (f == 2) ? 3 : f + 2;
            }
            if (tmp > 1 && pcnt < sizeof(primes)/sizeof(primes[0]))
                primes[pcnt++] = tmp;

            seq_t best = 0;
            for (size_t i = 0; i < pcnt; ++i) {
                uint64_t p = primes[i];
                seq_t cand = (seq_t)p;
                size_t idx = (size_t)cand;
                ensure_used(&used, &used_size, idx);
                while (used[idx]) {
                    cand += (seq_t)p;
                    idx = (size_t)cand;
                    ensure_used(&used, &used_size, idx);
                }
                if (best == 0 || cand < best)
                    best = cand;
            }
            val = best;
        }

        ensure_cache(&cache, &cache_size, cache_len + 1);
        cache[cache_len] = val;

        /* mark as used */
        size_t vidx = (size_t)val;
        ensure_used(&used, &used_size, vidx);
        used[vidx] = 1;

        ++cache_len;
    }
    return cache[n - 1];
}
