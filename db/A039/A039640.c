/*
 * A039640  Fixed point of "k -> k/2 or (k+1)/2 until result is prime",
 *          starting with prime(n)-1.
 * https://oeis.org/A039640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the fixed point of the map k -> ceil(k/2) applied *
 *            repeatedly to k = prime(n)-1 until a prime (or 1) is reached. *
 *            For prime(n)-1 = 1 this gives a(n)=1. * * The sequence is
 *            defined for n >= 1. The routine tolerates * non‑increasing calls
 *            by always recomputing from the nth prime. * No memo...
 * Range:     u8 n <= 54, u16 n <= 6926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9661 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

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
    /* start from the (n‑th) prime minus one */
    seq_t k = (seq_t)get_prime(n) - 1;

    /* Special case: if the starting value is 1, it is already a fixed point. */
    if (k == (seq_t)1)
        return k;

    /* Apply the transform until we reach a prime or the value 1. */
    while (k > (seq_t)1 && !is_prime((idx_t)k)) {
        if ((k & (seq_t)1) == (seq_t)0)          /* k even */
            k = k / (seq_t)2;
        else                                     /* k odd */
            k = (k + (seq_t)1) / (seq_t)2;
    }

    return k;   /* either a prime or 1 */
}
