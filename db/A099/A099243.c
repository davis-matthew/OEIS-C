/*
 * A099243  A variation on Flavius's sieve (A000960): Start with the primes;
 *          at the k-th sieving step, remove every p-th term of the sequence
 *          remaining after the (k-1)-st sieving step, where p is the k-th
 *          prime; iterate.
 * https://oeis.org/A099243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 609, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9767 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the definition of A099243: start with the prime
 * sequence and repeatedly delete every p-th element, where p is the k-th
 * prime.  The remaining elements are the terms of the sequence. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
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
    if (n == 0) {
        /* undefined for n = 0; return 0 as a safe value */
        return (seq_t)0;
    }

    /* Ensure we use 64‑bit arithmetic for index conversions where needed */
    idx_t idx = n;
    /* initial number of primes to consider */
    size_t L = (size_t)idx * 10;
    if (L < 100) L = 100;

    while (1) {
        /* Allocate array big enough to hold L primes */
        seq_t *seq = mem_new(seq_t, L);
        if (!seq) return (seq_t)0; /* allocation failure */

        /* Fill with the first L primes */
        for (size_t i = 0; i < L; ++i) {
            seq[i] = (seq_t)get_prime((idx_t)(i + 1));
        }

        size_t cur_sz = L;
        size_t step   = 1; /* step number, corresponds to the k‑th prime */

        /* Perform the deletion steps until the next prime > current size */
        while (1) {
            uint32_t p = get_prime((idx_t)step);
            if ((size_t)p > cur_sz)
                break;

            /* In‑place delete every p‑th element */
            size_t w = 0;
            for (size_t r = 0; r < cur_sz; ++r) {
                if (((r + 1) % p) != 0)
                    seq[w++] = seq[r];
            }
            cur_sz = w;
            ++step;
        }

        /* If we have enough terms, return the requested one */
        if (cur_sz >= (size_t)idx) {
            seq_t result = seq[(size_t)idx - 1];
            mem_free(seq);
            return result;
        }

        /* Not enough terms – enlarge L and retry */
        mem_free(seq);
        L <<= 1; /* double the capacity */
    }
}
