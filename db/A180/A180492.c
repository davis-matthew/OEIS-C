/*
 * A180492  Product of remainders of prime(n) mod k, for k =
 *          2,3,4,...,prime(n)-1.
 * https://oeis.org/A180492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A180492
 *  Formula:  a(n) = Π_{k=2}^{p_n-1} (p_n mod k),
 *  where p_n is the n‑th prime.
 *
 *  The function below obtains the n‑th prime via get_prime(n)
 *  (see ../utils/get_prime.h) and multiplies all remainders.
 *  It is straightforward, works for all supported seq_t types,
 *  and needs no auxiliary storage or memoisation.
 */
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.
       For n == 0 we return 0 (never called by the test harness). */
    if (n == 0) return 0;

    /* Get the n‑th prime.  get_prime returns a uint32_t. */
    uint32_t p = get_prime(n);

    seq_t prod = 1;                  /* start with multiplicative identity */

    /* Multiply (p % k) for every integer k = 2 … p‑1. */
    for (uint32_t k = 2; k < p; ++k) {
        prod *= (seq_t)(p % k);
    }

    return prod;
}
