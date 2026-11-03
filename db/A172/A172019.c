/*
 * A172019  Numbers k such that 4 divides phi(k) (i.e., A000010(k)).
 * https://oeis.org/A172019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth positive integer k such that 4 divides φ(k)
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 7363 (2026-11-03)
 * Helpers:   mem, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;

    /* produce terms until we have n */
    while (cache_len < n) {
        uint64_t cand = (uint64_t)next_k;
        uint64_t phi = totient_u64(cand);

        if ((phi % 4ULL) == 0) {
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    mem_free(cache);
                    exit(1);   /* unrecoverable memory failure */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;   /* may wrap for small seq_t types */
        }
        next_k++;
    }
    return cache[n - 1];
}
