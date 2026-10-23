/*
 * A167941  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 324*a(n-16) + 300*a(n-17) for n >= 18 Initial
 *            values: a(0)=1, a(1)=26, a(2)=650, a(3)=16250, a(4)=406250,
 *            a(5)=10156250, a(6)=253906250, a(7)=6347656250,
 *            a(8)=158691406250, a(9)=3967285156250, a(10)=99182128906250,
 *            a(11)=2479553222656250, a(12)=61988830566406250,
 *            a(13)=15497207...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t string_to_seq(const char *s) {
    seq_t val = 0;
    for (; *s; ++s) {
        val = val * (seq_t)10 + (seq_t)(*s - '0');
    }
    return val;
}

static const char *const init_str[16] = {
    "1","26","650","16250","406250",
    "10156250","253906250","6347656250","158691406250",
    "3967285156250","99182128906250","2479553222656250",
    "61988830566406250","1549720764160156250",
    "38743019104003906250","968575477600097656250"
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of populated entries, >=16 after init */
static size_t cache_cap = 0;   /* allocated capacity */

/* Prepare cache with first 18 terms (indices 0..17) */
static void init_cache(void) {
    /* Allocate at least 18 entries */
    cache_cap = 18;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    /* Fill known initial 16 terms */
    for (size_t i = 0; i < 16; ++i) {
        cache[i] = string_to_seq(init_str[i]);
    }
    /* Compute a(16) = 25*a(15) - 325 */
    seq_t a16 = cache[15] * (seq_t)25 - (seq_t)325;
    cache[16] = a16;
    /* Compute a(17) = 25*a(16) - 8125 */
    seq_t a17 = a16 * (seq_t)25 - (seq_t)8125;
    cache[17] = a17;
    cache_len = 18;
}

/* Ensure cache has terms up to index n (inclusive) */
static void ensure_cache(idx_t n) {
    if (cache_len == 0) {
        init_cache();
    }
    /* Grow cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    if ((size_t)n < cache_len) return;

    /* Compute terms from cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Recurrence applies for i >= 18 */
        seq_t val;
        if (i < 18) {
            /* All needed values already available (a0..a17) */
            val = cache[i];
        } else {
            val = cache[i-1] * (seq_t)25
                  - cache[i-16] * (seq_t)324
                  + cache[i-17] * (seq_t)300;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[(size_t)n];
}
