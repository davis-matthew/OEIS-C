/*
 * A092773  (Prime(prime(n))^2+1)/2.
 * https://oeis.org/A092773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Prime(Prime(n))^2+1)/2
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
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

/* Compute the n-th term of sequence A092773.
 *
 * We use the existing helper `get_prime(idx_t k)` which returns the
 * k‑th prime (1‑based indexing).  The result type `seq_t` handles
 * large integers, so the arithmetic below is safe for all indices
 * that fit in `idx_t`.  No memoisation is required – the helper
 * already caches primes internally.
 */

seq_t a(idx_t n)
{
    /* Ignore n == 0; behaviour for n < 1 is undefined per OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* First prime: p = prime(n). */
    seq_t p = get_prime(n);

    /* Second prime index is the value of p.  The value fits in idx_t
     * for all practical n (e.g., n <= 10^7 gives p <= 200 million). */
    idx_t idx_second = (idx_t)p;

    /* Second prime: q = prime(p). */
    seq_t q = get_prime(idx_second);

    /* Compute (q^2 + 1)/2. */
    seq_t q2 = q * q;
    seq_t result = (q2 + (seq_t)1) / (seq_t)2;

    return result;
}
