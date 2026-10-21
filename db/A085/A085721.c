/*
 * A085721  Semiprimes whose prime factors have an equal number of digits in
 *          binary representation.
 * https://oeis.org/A085721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 419, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085721: a(n) = n-th semiprime p*q such that
   binary_len(p) == binary_len(q)  (length = floor_log2 + 1). */
#include <stdint.h>
#include <stddef.h>

/* ------- Utility helpers -------- */
static inline unsigned bitlen_uint64(uint64_t v)
{
    /* number of bits needed to represent v, v > 0 */
    unsigned r = 0;
    while (v) { ++r; v >>= 1; }
    return r;
}

/* ------- Sieve of Eratosthenes for smallest prime factor -------- */
static uint32_t *spf = NULL;          /* lowest prime factor for each i  */
static size_t  spf_limit = 0;         /* allocated size - 1 */
static void init_sieve(size_t limit)
{
    if (limit <= spf_limit) return;   /* already large enough */
    /* free old array */
    if (spf) { free(spf); spf = NULL; }
    /* allocate (limit+1) */
    spf_limit = limit;
    spf = (uint32_t *)malloc((spf_limit + 1) * sizeof(uint32_t));
    if (!spf) return;                 /* allocation failure: caller must check */
    for (size_t i = 0; i <= spf_limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= spf_limit; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            for (size_t j = i * i; j <= spf_limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
}

/* ------- Cache of sequence terms -------- */
static seq_t *term_cache = NULL;      /* dynamic array of found terms  */
static size_t term_cnt  = 0;          /* number of cached terms       */
static uint32_t last_checked = 3;     /* last integer examined   */

/* append a new term to cache */
static void cache_push(seq_t val)
{
    /* grow if necessary */
    static size_t cache_cap = 0;
    if (term_cnt == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* silently ignore allocation failure */
        term_cache = tmp;
        cache_cap = new_cap;
    }
    term_cache[term_cnt++] = val;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    /* initialise once */
    static int init_done = 0;
    if (!init_done) {
        init_sieve(2000000);   /* start with a generous limit  */
        term_cache = NULL;
        term_cnt  = 0;
        last_checked = 3;
        init_done = 1;
    }

    /* extend cache until it contains the n-th term */
    while (term_cnt < (size_t)n) {
        ++last_checked;
        if (last_checked > spf_limit) {
            /* need larger sieve */
            init_sieve(spf_limit * 2);
        }
        uint32_t i = (uint32_t)last_checked;
        uint32_t p = spf[i];
        if (p != i) {                    /* not prime */
            uint32_t q = i / p;
            if (q >= p && spf[q] == q) { /* semiprime (p*q or p^2) */
                if (bitlen_uint64(p) == bitlen_uint64(q))
                    cache_push((seq_t)i);
            }
        }
    }

    return term_cache[n - 1];
}
