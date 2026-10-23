/*
 * A154893  Numbers whose number of proper divisors is not a prime number.
 * https://oeis.org/A154893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th positive integer whose number of proper divisors
 *            (i.e. divisor_count(m) – 1) is NOT a prime. Indexing starts at n
 *            = 1. The function stores found terms in a static cache so that
 *            calls in any order stay fast
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   /* provides uint64_t divisor_count_u64(uint64_t) */

/* helper to test primality of a small 64‑bit number */
static bool is_small_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of accepted numbers */
    static seq_t *cache   = NULL;  /* array of cached terms */
    static size_t cap     = 0;     /* allocated size of cache   */
    static size_t len     = 0;     /* number of filled entries  */
    static uint64_t tested_up_to = 0;  /* last integer examined   */

    /* generate terms until we have at least n entries */
    while (len < (size_t)n) {
        uint64_t cand = ++tested_up_to;                 /* candidate integer to test */
        uint64_t d    = divisor_count_u64(cand);          /* total number of divisors   */
        uint64_t proper = d - 1;                        /* proper divisors count     */

        if (!is_small_prime(proper)) {                  /* keep if not prime         */
            if (len >= cap) {                           /* grow cache if needed      */
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return cache[len - 1];         /* allocation failed          */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
