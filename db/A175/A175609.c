/*
 * A175609  The difference between maximal run length and minimal run length
 *          in binary representation of n-th prime.
 * https://oeis.org/A175609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = difference between maximal and minimal run lengths
   in the binary representation of the n-th prime.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bit_length.h"

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
    /* Get the n-th prime. */
    uint32_t prime = get_prime(n);
    seq_t val = (seq_t)prime;

    /* Number of bits in the binary representation. */
    idx_t len = bit_length(val);

    /* Run tracking. */
    seq_t max_run = 1;
    seq_t min_run = (seq_t)len;   /* initialized to upper bound */
    seq_t run    = 1;

    /* First bit (most significant). */
    int prev = (int)((val >> (len - 1)) & 1);

    /* Iterate over the remaining bits from most to least significant. */
    for (idx_t i = len - 1; i > 0; --i) {
        int bit = (int)((val >> (i - 1)) & 1);
        if (bit == prev) {
            run++;
        } else {
            if (run > max_run) max_run = run;
            if (run < min_run) min_run = run;
            run    = 1;
            prev   = bit;
        }
    }

    /* Update for the last run. */
    if (run > max_run) max_run = run;
    if (run < min_run) min_run = run;

    return max_run - min_run;
}
