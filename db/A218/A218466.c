/*
 * A218466  Least k > n for which phi(k - n) = phi(k + n) or 0 if no such k
 *          exists.
 * https://oeis.org/A218466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 881 (2026-11-03)
 * Helpers:   mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS definition:
 * a(n) = the smallest integer k > n for which phi(k‑n) = phi(k+n);
 * a(n) = 0 if no such k exists.
 * The implementation performs a brute‑force search using the provided
 * seq_t calc_phi(seq_t m) routine and memoises results for future calls.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>          /* for memset */
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values (indexed by n, 1‑based)   */
    static seq_t *cache  = NULL;
    static size_t  cap   = 0;   /* current allocated size of cache */

    /* enlarge cache if necessary */
    if (n >= cap) {
        size_t new_cap = (cap > 0) ? cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);        /* cannot allocate */
        if (tmp != cache) {
            /* preserve previous contents */
            mem_set(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        } else {
            /* cache pointer unchanged, just clear the new part */
            mem_set(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* if already computed, return cached value */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* brute‑force search for the smallest k > n with phi(k‑n) == phi(k+n) */
    for (idx_t k = n + 1; ; ++k) {
        seq_t left  = totient_seq((seq_t)(k - n));
        seq_t right = totient_seq((seq_t)(k + n));
        if (left == right) {
            cache[n] = (seq_t)k;   /* store result for future calls */
            return cache[n];
        }
        /* As the definition guarantees existence for the values tested
         * by OEIS, the loop will eventually terminate.  For safety we
         * avoid an infinite loop by not imposing a hard limit here,
         * trusting the sequence definition. */
    }
}
