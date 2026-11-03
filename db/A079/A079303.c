/*
 * A079303  a(n) = ((p-1)!/(2*p^2)) * Sum_{j=1..p-1} 1/j where p is the n-th
 *          prime.
 * https://oeis.org/A079303
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p-1)!/(2*p^2)) * Sum_{j=1..p-1} 1/j where p =
 *            A000040(n) is the n‑th prime. The computation uses the
 *            recurrence fac_i = (i-1)! , val_i = fac_i * H_{i-1} (initially 0
 *            for i=1) For i = 1 .. p-1 fac_next = fac_i * i // (i)! val_next
 *            = i * val_i + fac_i // i! * H_i After the loop, val_{p-1}...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 72 (2026-11-01)
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
    /* A000040(1)=2, A000040(2)=3 – the sequence is defined for n≥3. */
    if (n < 3)
        return (seq_t)0;

    uint32_t p = get_prime(n);                 /* the n‑th prime          */
    const unsigned __int128 denom128 = (unsigned __int128)2 * p * p;

    /* Branch according to the size of seq_t. */
    if (sizeof(seq_t) <= 8)                      /* 64‑bit or smaller */
    {
        unsigned __int128 fac = 1;
        unsigned __int128 val = 0;

        for (uint32_t i = 1; i < p; ++i)
        {
            unsigned __int128 fac_next = fac * i;          /* i!                     */
            unsigned __int128 val_next = (unsigned __int128)i * val + fac;
            fac  = fac_next;
            val  = val_next;
        }
        val /= denom128;                                /* exact division          */
        return (seq_t)val;
    }
    else                                            /* arbitrary‑precision   */
    {
        seq_t fac = (seq_t)1;
        seq_t val = (seq_t)0;

        for (uint32_t i = 1; i < p; ++i)
        {
            seq_t fac_next = fac * (seq_t)i;          /* i!                      */
            val = (seq_t)i * val + fac;               /* i! * H_i                */
            fac = fac_next;
        }
        seq_t denom = (seq_t)2 * (seq_t)p * (seq_t)p;
        return val / denom;
    }
}
