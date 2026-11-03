/*
 * A155480  Composites n with identical largest digits of n and n-th prime.
 * https://oeis.org/A155480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155480: composites n with identical largest digits of n and n‑th prime. */
#include <stdbool.h>
#include <stdint.h>
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

/* Return largest decimal digit of x. */
static int largest_digit(seq_t x)
{
    int maxd = 0;
    while (x > 0)
    {
        seq_t d = x % 10;
        int di = (int)(unsigned long long)d;
        if (di > maxd) maxd = di;
        x /= 10;
    }
    return maxd;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for zero index */

    idx_t count = 0;
    idx_t m = 4;   /* first composite number */

    for (;; ++m)
    {
        if (!is_prime(m))
        {
            int d_m = largest_digit((seq_t)m);
            uint32_t p = get_prime((idx_t)m);
            int d_p = largest_digit((seq_t)p);
            if (d_m == d_p)
            {
                ++count;
                if (count == n)
                    return (seq_t)m;
            }
        }
    }
}
