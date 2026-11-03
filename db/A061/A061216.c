/*
 * A061216  a(n) = product of all even numbers between n-th prime and (n+1)-st
 *          prime.
 * https://oeis.org/A061216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all even numbers strictly between p_n and
 *            p_{n+1}
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 98, u128 n <= 216,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 1881 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A061216) */
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
    /* handle n >= 1; a(1) = 1 by the loop below */
    uint32_t p1 = get_prime(n);   /* nth prime */
    uint32_t p2 = get_prime(n + 1); /* (n+1)-th prime */

    seq_t prod = 1;

    /* First even integer greater than p1 */
    idx_t start = (idx_t)p1 + 1;
    if ((start & 1) != 0)
        start += 1;          /* make even */

    /* Last integer less than p2 */
    idx_t end = (idx_t)p2 - 1;

    /* Multiply all even numbers in [start, end] */
    for (idx_t v = start; v <= end; v += 2) {
        prod *= (seq_t)v;
    }

    return prod;
}
