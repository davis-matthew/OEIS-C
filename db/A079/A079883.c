/*
 * A079883  a(1) = 1; a(n) = prime(n) - prime(n-1)* a(n-1) if n > 1.
 * https://oeis.org/A079883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = prime(n) - prime(n-1) * a(n-1) (n > 1)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* Undefined for n < 1, value irrelevant */

#ifdef __cplusplus
    /* Cache previously computed terms using std::vector for efficiency */
    static std::vector<seq_t> cache;
    static idx_t computed = 0;   /* index of largest term computed */

    if (computed == 0) {
        /* Initialise cache with a(1) */
        cache.resize(2);          /* indices 0 and 1 */
        cache[1] = (seq_t)1;
        computed = 1;
    }

    if (n > computed) {
        cache.resize((size_t)n + 1);  /* Ensure space for indices up to n */

        for (idx_t k = computed + 1; k <= n; ++k) {
            uint32_t pk  = get_prime(k);
            uint32_t pk1 = get_prime(k - 1);
            cache[k] = (seq_t)pk - (seq_t)pk1 * cache[k - 1];
        }
        computed = n;
    }

    return cache[n];
#else
    /* Plain iterative computation for C builds (no caching) */
    seq_t value = (seq_t)1;  /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        uint32_t pk  = get_prime(k);
        uint32_t pk1 = get_prime(k - 1);
        value = (seq_t)pk - (seq_t)pk1 * value;
    }

    return value;
#endif
}
