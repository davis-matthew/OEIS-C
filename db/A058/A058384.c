/*
 * A058384  Largest power of 2 which is a divisor of p(n)-1, where p(n) = n-th
 *          prime.
 * https://oeis.org/A058384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of 2 that divides p(n)-1, where p(n) is the
 *            n-th prime (offset 1)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9776 (2026-11-01)
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
    /* If n < 1 we return 0; behaviour for indices below the offset
       does not matter according to the problem statement. */
    if (n < 1)
        return 0;

    /* Get the n-th prime.  get_prime returns a 32‑bit unsigned prime. */
    uint32_t p = get_prime(n);

    /* Compute p-1 and find its largest power of two divisor
       by shifting out all trailing zeros. */
    seq_t m = (seq_t)p - 1;   /* p-1  */
    seq_t power = 1;          /* power of two factor */

    while ((m & 1) == 0) {    /* while the least significant bit is 0 */
        power <<= 1;          /* multiply power by 2 */
        m >>= 1;              /* shift right to remove that zero bit */
    }
    return power;             /* largest power of 2 dividing p-1 */
}
