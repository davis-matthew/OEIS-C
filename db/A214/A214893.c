/*
 * A214893  Conway's subprime Fibonacci sequence starting with (18, 5).
 * https://oeis.org/A214893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Conway's subprime Fibonacci sequence starting with (18,5).
 *            Recurrence: a(1)=18, a(2)=5. For n≥3: t = a(n-1)+a(n-2); if t is
 *            composite, t = t / lpf(t), where lpf is the least prime factor
 * Range:     u8 n <= 12, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Helper: find least prime factor of a composite number using a growing prime list */
static seq_t _lpf(seq_t x) {
    /* odd check */
    if ((x & (seq_t)1) == 0) return (seq_t)2;

    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    seq_t sq = floor_sqrt(x);
    idx_t lim = (idx_t)sq;
    if (lim > primes_limit) {
        free(primes);
        build_primes_upto(lim, &primes, &primes_cnt);
        primes_limit = lim;
    }
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pt = (seq_t)p;
        if (x % pt == 0) return pt;
    }
    /* Should not happen for composite x */
    return x;
}

/* Main function: compute the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined but safe guard */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache_size == 0) {
        /* allocate initial space for first two terms */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    if ((idx_t)n > cache_size) {
        size_t new_size = (size_t)n;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));

        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t tmp = cache[i - 1] + cache[i - 2];
            if (!isPrimeGeneric(tmp)) {
                seq_t lpf = _lpf(tmp);
                tmp = tmp / lpf;
            }
            cache[i] = tmp;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n - 1];
}
