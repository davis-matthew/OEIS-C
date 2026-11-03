/*
 * A096060  (2^(p-1)-1)/(3*p) where p = prime(n).
 * https://oeis.org/A096060
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(p-1)-1) / (3*p), where p = prime(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 79 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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
    /* The sequence is defined for n >= 3.  For smaller indices return 0. */
    if (n < 3)
        return 0;

    /* Get the n‑th prime.  get_prime returns a 32‑bit integer. */
    uint64_t p = get_prime(n);

    /* For 64‑bit or smaller seq_t we can compute 2^(p-1) exactly in a
       128‑bit temporary when p-1 <= 127.  This covers all cases where
       the final result fits in seq_t. */
    if (sizeof(seq_t) <= sizeof(unsigned long long) && p <= 127)
    {
        /* power = 2^(p-1)  (fits in 128 bits) */
        unsigned __int128 power = ((unsigned __int128)1) << (p - 1);
        unsigned __int128 numerator = power - 1;
        unsigned __int128 denominator = ((unsigned __int128)3) * p;
        return (seq_t)(numerator / denominator);
    }
    else
    {
        /* Use the arbitrary‑precision helper for larger p or larger
           seq_t types. */
        seq_t power = ipow((seq_t)2, (seq_t)(p - 1));
        seq_t numerator = power - 1;
        seq_t denominator = (seq_t)3 * p;
        return numerator / denominator;
    }
}
