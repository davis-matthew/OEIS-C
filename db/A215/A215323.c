/*
 * A215323  Primes congruent to {1, 2, 3, 4, 6} mod 13.
 * https://oeis.org/A215323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 888 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute: a(n) is the n‑th prime p such that p mod 13 ∈ {1,2,3,4,6}. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static size_t cap      = 0;
    static size_t len      = 0;
    static uint64_t next_candidate = 2;   /* start searching from 2 */

    while (len < n)
    {
        uint64_t p = next_candidate;

        for (;; ++p)
        {
            /* test primality (2 is handled specially) */
            if (p == 2 || is_prime_seq(p))
            {
                uint64_t r = p % 13;
                if (r == 1 || r == 2 || r == 3 || r == 4 || r == 6)
                {
                    /* grow cache if necessary */
                    if (len + 1 > cap)
                        ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));

                    cache[len++] = (seq_t)p;
                    next_candidate = p + 1;   /* start next search after this prime */
                    break;
                }
            }
        }
    }

    return cache[n - 1];
}
