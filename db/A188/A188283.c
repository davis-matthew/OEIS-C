/*
 * A188283  Numbers k such that iterations for the map r -> A061602(r)
 *          starting with k ends with a fixed point (factorions A014080).
 * https://oeis.org/A188283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over integers and keep those that eventually reach a
   factorion (fixed point of the digit‑factorial sum map).  Cache results so
   that repeated calls to a() run in O(1) after the cache is built. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

static const unsigned int fact_digits[10] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
};

/* Does start eventually reach a factorion? */
static bool leads_to_factorion(seq_t start) {
    seq_t val = start;
    seq_t visited[64];
    size_t vlen = 0;

    for (;;) {
        /* fixed points (factorions) */
        if (val == (seq_t)1 ||
            val == (seq_t)2 ||
            val == (seq_t)145 ||
            val == (seq_t)40585) {
            return true;
        }

        /* detect repetition (non‑fixed loop) */
        size_t i;
        for (i = 0; i < vlen; ++i)
            if (visited[i] == val)
                return false;
        if (vlen < sizeof(visited) / sizeof(visited[0]))
            visited[vlen++] = val;

        /* next value: sum of factorial of decimal digits */
        uint8_t buf[64];
        size_t len;
        get_digits(val, buf, &len);
        seq_t sum = 0;
        for (i = 0; i < len; ++i)
            sum += (seq_t)fact_digits[buf[i]];
        val = sum;
    }
}

/* cache for computed terms */
static seq_t *cache      = NULL;
static size_t cache_n    = 0;  /* number of terms stored */
static size_t cache_cap  = 0;  /* allocated capacity */
static seq_t last_tested = 0;  /* next candidate to test */

/* Ensure cache has room for at least 'need' elements */
static void ensure_cache(size_t need) {
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based index) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, guard */
    ensure_cache((size_t)n);
    while (cache_n < (size_t)n) {
        if (leads_to_factorion(last_tested))
            cache[cache_n++] = last_tested;
        last_tested += (seq_t)1;
    }
    return cache[(size_t)(n - 1)];
}
