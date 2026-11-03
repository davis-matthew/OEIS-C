/*
 * A228529  a(n) = prime(n*prime(n)).
 * https://oeis.org/A228529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n*prime(n))
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

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

/* Return the k-th prime (k >= 1).  Uses binary search over the prime counting
   function compute_pi.  Limits: k <= 10^9 works within 64‑bit indices. */
static seq_t nth_prime(idx_t k)
{
    /* Handle the first few primes by a small table. */
    if (k <= 6) {
        static const seq_t sp[6] = {2,3,5,7,11,13};
        return sp[k-1];
    }

    idx_t low  = k + 1;          /* lower bound for the k-th prime */
    idx_t high = k * k;          /* trivial upper bound (k^2) */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cnt = prime_pi(mid);
        if (cnt >= (seq_t)k) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; by definition a(1)=primer(1*primer(1)) = 3. */
    if (n == 0) return 0;  /* undefined for n=0, harmless for tests */

    /* First, get the n‑th prime.  get_prime returns a uint32_t, which
       comfortably fits into idx_t for n up to 10^4. */
    idx_t p_n = (idx_t)get_prime(n);

    /* Compute the index m = n * p_n. */
    idx_t m = n * p_n;

    /* Return the m‑th prime. */
    return nth_prime(m);
}
