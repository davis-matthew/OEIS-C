/*
 * A138659  Primes p such that 60*p - 1 and 60*p + 1 are twin primes.
 * https://oeis.org/A138659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Search for primes p such that 60*p-1 and 60*p+1 are primes
 * Range:     u8 n <= 12, u16 n <= 423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 8982 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;
static size_t len   = 0;
static size_t cap   = 0;
static uint64_t next_candidate = 3;   /* start from the first odd prime >2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1, return 0 */

    /* Grow the cache until we have at least n terms. */
    while (len < n) {
        /* Find the next qualifying prime. */
        while (true) {
            uint64_t p = next_candidate;
            next_candidate += 2;        /* skip even numbers */

            if (is_prime(p)) {
                /* Calculate 60*p-1 and 60*p+1 safely using 128-bit arithmetic. */
                uint64_t a60 = (uint64_t)(((__uint128_t)60 * p) - 1);
                uint64_t b60 = (uint64_t)(((__uint128_t)60 * p) + 1);

                if (is_prime(a60) && is_prime(b60)) {
                    seq_t val = (seq_t)p;
                    append_candidate(&cache, &len, &cap, val);
                    break;  /* found a term, restart outer while to check len */
                }
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
