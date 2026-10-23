/*
 * A139946  Primes of the form 14x^2+14xy+29y^2.
 * https://oeis.org/A139946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form 14*x^2 + 14*x*y + 29*y^2.
   Such primes are exactly the primes that are congruent to one of the
   residues
   29,65,113,197,233,317,401,449,473,533,
   617,641,653,725,737,785,809,821,989,1061,
   1145,1229,1289,1397 modulo 1428. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

static const uint16_t RESIDUES[24] = {
    29,65,113,197,233,317,401,449,473,533,
    617,641,653,725,737,785,809,821,989,1061,
    1145,1229,1289,1397
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_prime_checked = 2; /* prime before first candidate */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t p = next_prime(last_prime_checked + 1);
        last_prime_checked = p;

        uint64_t r = p % 1428;
        bool ok = false;
        for (int i = 0; i < 24; ++i) {
            if (r == RESIDUES[i]) {
                ok = true;
                break;
            }
        }
        if (!ok)
            continue;

        if (cache_len + 1 > cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)p;
    }

    return cache[n - 1];
}
