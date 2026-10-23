/*
 * A131565  Numbers n such that 3*n^2 + 5*n +1 is either a prime or a power of
 *          three multiplied with a prime.
 * https://oeis.org/A131565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth positive integer m such that
 * 3*m^2 + 5*m + 1 is prime or a power of 3 multiplied by a prime. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;          /* array of sequence values */
    static size_t cache_len = 0;         /* number of terms stored */
    static size_t cache_cap = 0;         /* storage capacity of the cache */
    static uint64_t last_candidate = 1;  /* highest m that has been evaluated */

    /* grow cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        uint64_t candidate = last_candidate + 1;
        seq_t m = (seq_t)candidate;

        /* calculate f = 3*m^2 + 5*m + 1 */
        seq_t val = (seq_t)3 * m * m + (seq_t)5 * m + (seq_t)1;

        /* check if val is prime */
        bool good = false;
        if (isPrimeGeneric(val)) {
            good = true;
        } else if (val % (seq_t)3 == (seq_t)0) {
            /* factor out all powers of 3 */
            seq_t tmp = val;
            while (tmp % (seq_t)3 == (seq_t)0) {
                tmp /= (seq_t)3;
            }
            if (tmp > (seq_t)1 && isPrimeGeneric(tmp)) {
                good = true;
            }
        }

        if (good) {
            /* ensure sufficient storage */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!new_cache) abort();   /* simple OOM handling */
                cache = new_cache;
            }
            cache[cache_len++] = m;
        }
        last_candidate = candidate;
    }

    return cache[n - 1];
}
