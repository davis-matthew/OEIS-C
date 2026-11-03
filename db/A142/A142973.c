/*
 * A142973  a(n) = the smallest positive integer that does not divide any
 *          integer k, where the n-th prime <= k <= the (n+1)th prime.
 * https://oeis.org/A142973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m that does not divide any
 *            integer k with the n-th prime <= k <= the (n+1)-th prime
 * Range:     u8 n <= 53, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    if (n == 0) {
        /* Undefined for indices below the offset */
        return 0;
    }

    /* Obtain the n-th and (n+1)-th primes */
    uint32_t p_n   = get_prime(n);     /* 1‑indexed */
    uint32_t p_n1  = get_prime(n + 1);

    seq_t m = 1;

    /* Search for the smallest m with no multiple in [p_n, p_n1] */
    while (1) {
        seq_t a_start = (seq_t)p_n;
        seq_t a_end   = (seq_t)p_n1;

        /* First multiple of m that is >= a_start */
        seq_t first_multiple = ((a_start + m - 1) / m) * m;

        if (first_multiple > a_end) {
            /* No multiple of m appears in the interval */
            return m;
        }
        m++;
    }
}
