/*
 * A110024  Smallest primes starting a complete three iterations Cunningham
 *          chain of the second kind.
 * https://oeis.org/A110024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for primes p such that
 * p, 2p−1, 4p−3, 8p−7 are prime,
 * 16p−15 is composite,
 * and (p+1)/2 is composite (ensuring p is the first term of the chain). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0, cache_size = 0;

    static uint64_t cand = 3;                     /* next odd candidate */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        for (;;) {
            /* Must be prime. */
            if (is_prime_u64(cand)) {
                /* Ensure we are not a continuation of a longer chain. */
                if (is_prime_u64((cand + 1) / 2)) {
                    cand += 2;
                    continue;
                }

                uint64_t q1 = 2 * cand - 1;    /* 2p-1   */
                if (!is_prime_u64(q1)) {
                    cand += 2;
                    continue;
                }

                uint64_t q2 = 4 * cand - 3;    /* 4p-3   */
                if (!is_prime_u64(q2)) {
                    cand += 2;
                    continue;
                }

                uint64_t q3 = 8 * cand - 7;    /* 8p-7   */
                if (!is_prime_u64(q3)) {
                    cand += 2;
                    continue;
                }

                uint64_t q4 = 16 * cand - 15;  /* 16p-15 */
                if (is_prime_u64(q4)) {
                    cand += 2;
                    continue;                 /* would extend chain to length 5 */
                }

                /* All conditions satisfied – add to cache. */
                push_seq(&cache, &cache_cap, &cache_size, (seq_t)cand);
                cand += 2;   /* Move to next candidate. */
                break;
            }
            cand += 2;                           /* skip even numbers */
        }
    }

    return cache[n - 1];
}
