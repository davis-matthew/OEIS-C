/*
 * A078838  a(n) = Sum_{k=1..(p-1)*(p-2)} floor((k*p)^(1/3)) where p is the
 *          n-th prime.
 * https://oeis.org/A078838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (1/4)*(3*p-5)*(p-2)*(p-1) where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       (the offset is 1, so this value is undefined). */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n-th prime.  get_prime returns a 32‑bit unsigned
       integer which is sufficient for all n we care about (p grows
       slower than n log n). */
    uint32_t p = get_prime(n);

    /* Compute the formula using seq_t arithmetic.  The intermediate
       product fits in seq_t; the division by 4 is exact because the
       expression is always divisible by 4. */
    seq_t t1 = (seq_t)(3 * (seq_t)p - 5);
    seq_t t2 = (seq_t)(p - 2);
    seq_t t3 = (seq_t)(p - 1);
    seq_t prod = t1 * t2 * t3;
    return prod / (seq_t)4;
}
