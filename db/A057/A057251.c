/*
 * A057251  Numbers n such that n | 11^n + 10^n + 9^n + 8^n + 7^n.
 * https://oeis.org/A057251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that n | (11^n + 10^n + 9^n + 8^n + 7^n).
   Computed by incrementally testing integers and caching all
   solutions found so far.  The function is memoised, so repeated
   calls for different n return instantly. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* cache of all solutions discovered so far */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_len = 0;          /* number of values stored in cache */
static idx_t next_candidate = 1;     /* next integer to test for the property */

/* helper: returns the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */

    /* extend the cache until we have at least n terms */
    if (n > cache_len) {
        const seq_t bases[5] = {11, 10, 9, 8, 7};
        while (cache_len < n) {
            uint64_t mod = next_candidate;   /* use a larger type for accumulation */
            unsigned long long sum = 0;

            for (int i = 0; i < 5; ++i) {
                /* compute base^mod (mod next_candidate) */
                seq_t term = mod_pow(bases[i], next_candidate, (seq_t)mod);
                sum += (unsigned long long)term;
                /* reduce modulo mod (summation is always < 2*mod) */
                if (sum >= mod) sum -= mod;
            }

            /* if the condition holds, store this candidate */
            if (sum == 0) {
                if (cache_len == cache_capacity) {
                    size_t new_cap = cache_capacity == 0 ? 8 : cache_capacity * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* fatal: cannot allocate more memory */
                    cache = tmp;
                    cache_capacity = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
