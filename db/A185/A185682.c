/*
 * A185682  Irregular triangle, read by rows, of primes with prefix n and
 *          digits "1" appended, otherwise 0.
 * https://oeis.org/A185682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, tabf
 * Formula:   computed by brute‑force search: for each integer i=1,2,… compute
 *            primes of the form i*10 + 1 and i*100 + 11 until a composite is
 *            found. Output the primes; if no prime for i, output 0
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_base = 1;

    while (cache_len < (size_t)n) {
        seq_t cand1 = (seq_t)next_base * 10 + 1;
        if (is_prime((idx_t)cand1)) {
            /* Store cand1 */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 128;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand1;

            /* Second candidate: base followed by two '1's */
            seq_t cand2 = (seq_t)next_base * 100 + 11;
            if (is_prime((idx_t)cand2)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? 2 * cache_cap : 128;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand2;
            }
        } else {
            /* No prime for this base, output 0 */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 128;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)0;
        }
        ++next_base;
    }

    return cache[n - 1];
}
