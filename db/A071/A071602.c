/*
 * A071602  Sum of the reverses of the first n primes.
 * https://oeis.org/A071602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} reverse_decimal(get_prime(k))
 * Range:     u8 n <= 9, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9617 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/reverse_digits.h"
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

seq_t a(idx_t n)
{
    /* Static cache for cumulative sums a(0..computed) */
    static seq_t *cache = NULL;
    static idx_t computed = 0;

    if (n == 0) {
        return 0;        /* undefined in OEIS but safe */
    }

    if (n > computed) {
        /* Expand cache to accommodate index n */
        size_t needed = (size_t)n + 1; /* include a(0) */
        seq_t *new_cache = mem_resize(seq_t, cache, needed);
        if (!new_cache) {
            abort();  /* memory allocation failure */
        }
        cache = new_cache;

        /* Initialize base case if not yet set */
        if (computed == 0) {
            cache[0] = 0;
        }

        /* Compute new cumulative sums */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t prime = (seq_t)get_prime(i);
            seq_t rev   = reverse_digits(prime);
            cache[i] = cache[i - 1] + rev;
        }
        computed = n;
    }

    return cache[n];
}
