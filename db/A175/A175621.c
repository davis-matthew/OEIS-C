/*
 * A175621  a(n) = 2^(composite(n)-4) mod composite(n).
 * https://oeis.org/A175621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2^(composite(n)-4) mod composite(n)
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 959 (2026-11-01)
 * Helpers:   is_prime_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"

/* Computes the n-th term of OEIS sequence A175621.
   n is 1‑based; for n < 1 the behaviour is unspecified. */
seq_t a(idx_t n)
{
    /* Static cache of composite numbers (as uint64_t for efficiency). */
    static uint64_t *comps   = NULL;      /* array of composite numbers   */
    static idx_t   comps_sz = 0;          /* number of composites cached */
    static idx_t   comps_cap = 0;         /* current capacity of the array */
    static uint64_t last_seen = 3;        /* last integer checked          */

    /* Grow the cache until it contains at least n composites. */
    while (comps_sz < n) {
        ++last_seen;
        if (last_seen < 4)            /* 1,2,3 are not composite */
            continue;

        /* A composite if it is not prime. */
        if (!is_prime_seq((seq_t)last_seen)) {
            /* Ensure there is space for one more composite. */
            if (comps_sz == comps_cap) {
                idx_t new_cap = comps_cap ? comps_cap * 2 : 4;
                uint64_t *tmp = (uint64_t *)realloc(comps, new_cap * sizeof(uint64_t));
                if (!tmp)               /* allocation failure – abort */
                    return (seq_t)0;
                comps = tmp;
                comps_cap = new_cap;
            }
            comps[comps_sz++] = last_seen;
        }
    }

    /* Retrieve the n‑th composite number (1‑based). */
    uint64_t comp = comps[n - 1];

    /* Compute a(n) = 2^(comp-4) mod comp. */
    uint64_t exp = comp - 4;                 /* safe: comp ≥ 4 */
    return powmod((seq_t)2, exp, (seq_t)comp);
}
