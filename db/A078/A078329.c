/*
 * A078329  Primes p such that mu(p+1)=-1, where mu denotes the moebius
 *          function.
 * https://oeis.org/A078329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9503 (2026-11-03)
 * Helpers:   is_prime, mem, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that μ(p+1) = -1 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mobius.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static storage for already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_checked = 1;

    if (n == 0) return 0;          /* offset is 1; value for n<1 is unused */

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        if (!is_prime(cand))
            continue;

        if ((int)mobius(cand + 1) == -1) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
