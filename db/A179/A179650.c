/*
 * A179650  a(n) = (n-th prime) mod (n-th nonprime).
 * https://oeis.org/A179650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th prime) mod (n-th nonprime)
 * Range:     u8 n <= 54, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

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

/* Cache for nonprime numbers (composites plus 1). */
static seq_t *nonprime_cache   = NULL;   /* array of seq_t */
static size_t nonprime_cap     = 0;       /* allocated capacity */
static idx_t  nonprime_filled  = 0;       /* number of elements stored */
static idx_t  next_check       = 1;       /* next integer to test for nonprime */

/* Ensure that nonprime_cache holds at least ‘need’ elements. */
static void
ensure_nonprime(idx_t need)
{
    if (nonprime_filled >= need)
        return;

    /* Grow capacity to a power of two at least ‘need’ */
    size_t cap = nonprime_cap ? nonprime_cap : 128;
    while (cap < (size_t)need)
        cap <<= 1;

    if (cap != nonprime_cap) {
        seq_t *tmp = mem_resize(seq_t, nonprime_cache, cap);
        if (!tmp)          /* allocation failed – leave cache unmodified */
            return;
        nonprime_cache = tmp;
        nonprime_cap   = cap;
    }

    /* Append nonprime numbers until we have ‘need’ of them. */
    while (nonprime_filled < need) {
        if (!is_prime(next_check))
            nonprime_cache[nonprime_filled++] = (seq_t)next_check;
        ++next_check;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety for out-of-range */

    seq_t prime = (seq_t)get_prime(n);           /* nth prime (1‑based) */
    ensure_nonprime(n);                          /* ensure nth nonprime cached */
    seq_t nonprime = nonprime_cache[(size_t)(n - 1)]; /* nth nonprime */
    return prime % nonprime;
}
