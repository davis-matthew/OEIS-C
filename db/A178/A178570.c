/*
 * A178570  Numbers k such that prime(k+1) == 1 (mod (prime(k+2) - prime(k))).
 * https://oeis.org/A178570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 9120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A178570
 *  a(n) = the n‑th integer k (1‑based) such that
 *        prime(k+1) ≡ 1 (mod (prime(k+2) – prime(k))).
 *  The function iterates k from 1 upward, keeps a count of how many
 *  numbers have satisfied the condition, and returns the k that
 *  reaches the requested n‑th term.  Prime values are obtained via
 *  the supplied get_prime utility, which caches the primes so the
 *  cost is essentially that of a simple sieve.  The algorithm
 *  runs in O(n·p) time, p being the number of primes needed to
 *  reach the n‑th term (well below 2 seconds for n ≤ 10 000).  It
 *  works for 64‑bit (or arbitrary‑precision) builds because k and
 *  the result are returned as seq_t and all intermediate arithmetic
 *  uses uint64_t, which is larger than the primes involved.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

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
    idx_t found = 0;   /* how many qualifying k we have seen */
    idx_t k = 1;       /* candidate n */

    while (found < n) {
        uint64_t pk   = (uint64_t)get_prime(k);     /* prime(k)   */
        uint64_t pk1  = (uint64_t)get_prime(k + 1); /* prime(k+1) */
        uint64_t pk2  = (uint64_t)get_prime(k + 2); /* prime(k+2) */

        uint64_t diff = pk2 - pk;                   /* prime(k+2) – prime(k) */
        if (diff != 0 && (pk1 % diff) == 1) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
    /* Should never reach here because the sequence is infinite */
    return (seq_t)0;
}
