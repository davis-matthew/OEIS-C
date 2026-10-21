/*
 * A240709  Primes p such that no number among p+-6 and p+-12 is also a prime.
 * https://oeis.org/A240709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that none of p±6 or p±12 is also a prime. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /*  Cache of the terms that have already been found.  */
    static seq_t *cache = NULL;
    static size_t  len   = 0;   /* number of terms stored                 */
    static size_t  cap   = 0;   /* size of the allocated cache (sufficiently large) */
    static uint64_t next = 1;   /* prime to start searching from           */

    /* Allocate the cache only on the first call.  A size of 20000 is
       sufficient to hold more than ten thousand terms; this keeps the
       implementation simple and avoids a dynamic reallocation helper.  */
    if (!cache) {
        cap   = 20000;
        cache = seq_alloc(cap);
    }

    /* Generate terms up to the requested index n.  */
    while (len < (size_t)n) {
        uint64_t p = next_prime(next);
        next = p;  /* next prime occurrence for subsequent searches          */

        /* test the four neighbours ±6, ±12                                 */
        bool ok = true;
        const uint64_t offs[2] = {6, 12};

        for (int i = 0; i < 2 && ok; ++i) {
            uint64_t o = offs[i];

            /* p + o */
            if (is_prime(p + o)) {
                ok = false;
                break;
            }

            /* p – o – only if p > o to avoid underflow              */
            if (p > o && is_prime(p - o)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            cache[len++] = (seq_t)p;
        }
    }

    /* Indices are 1‑based in OEIS.  */
    return cache[(size_t)n - 1];
}
