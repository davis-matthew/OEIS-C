/*
 * A067161  a(n) = prime(sigma(n)).
 * https://oeis.org/A067161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(A000203(n))
 * Range:     u8 n <= 16, u16 n <= 1978, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9111 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

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
    if (n < 1) return 0;   /* undefined for non‑positive indices */

    /* Compute sigma(n): sum of divisors of n */
    uint64_t num = (uint64_t)n;
    size_t  cnt  = 0;
    uint64_t prime[128], exp[128];

    factor_u64(num, prime, exp, &cnt);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime[i];
        uint64_t e = exp[i];

        /* term = (p^(e+1) - 1)/(p - 1) */
        seq_t term_num = ipow_u64_idx(p, e + 1) - 1;
        seq_t term_den = (seq_t)(p - 1);
        sigma *= term_num / term_den;
    }

    /* sigma(1) = 1 is handled by the loop (cnt == 0) → sigma == 1 */

    /* Find the sigma(n)-th prime */
    idx_t prime_index = (idx_t)sigma;
    uint32_t prime_val = get_prime(prime_index);

    return (seq_t)prime_val;
}
