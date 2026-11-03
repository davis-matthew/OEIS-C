/*
 * A176815  Radius of the second-shortest interval (of positive length)
 *          centered at prime(n) that has prime endpoints, (zero if no
 *          solution exists).
 * https://oeis.org/A176815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176815:
 * Radius of the second-shortest interval centered at the n-th prime
 * that has prime endpoints; zero if no such interval exists. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* n‑th prime (1‑based) */
    uint32_t prim = get_prime(n);
    uint64_t p = prim;

    seq_t first  = 0;  /* radius of the first valid interval */
    seq_t second = 0;  /* radius of the second valid interval (answer) */

    for (uint64_t r = 1; ; ++r)
    {
        /* left endpoint becomes < 2 when r >= p‑1 */
        if (p <= r + 1)
            break;

        uint64_t left  = p - r;
        uint64_t right = p + r;

        if (is_prime((idx_t)left) && is_prime((idx_t)right))
        {
            if (first == 0)
                first = (seq_t)r;
            else
            {
                second = (seq_t)r;
                break;
            }
        }
    }

    return second;  /* zero if a second interval does not exist */
}
