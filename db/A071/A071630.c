/*
 * A071630  Number of terms in the set InvPhi(8*prime(n)), where prime(n) is
 *          the n-th prime.
 * https://oeis.org/A071630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9911 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts, for a given n, the number of solutions
   x with φ(x) = 8·prime(n).  It precomputes Euler's totient values up
   to a fixed limit L, stores in an array the number of occurrences of
   each totient value, and then answers each query in O(1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Adjust the limit if higher n are requested.  It must be > 8·prime(n)
   and large enough to cover all solutions of φ(x)=8·prime(n).  For
   n ≤ 10000 the bound 5 000 000 is safe. */
static const size_t LIM = 5000000;

static uint64_t *phi_arr      = NULL;        /* φ(i) for 1 ≤ i ≤ LIM */
static seq_t   *occurrences  = NULL;        /* count for each totient value */
static bool     initialized  = false;

/* One‑time initialization: compute φ(i) by sieve and count
   occurrences. */
static void init(void)
{
    if (initialized) return;

    phi_arr = (uint64_t *)malloc((LIM + 1) * sizeof(uint64_t));
    if (!phi_arr) return;          /* allocation failed */

    /* φ(i) sieve */
    for (size_t i = 0; i <= LIM; ++i) phi_arr[i] = (uint64_t)i;
    phi_arr[0] = 0;
    for (size_t i = 2; i <= LIM; ++i) {
        if (phi_arr[i] == (uint64_t)i) {        /* i is prime */
            for (size_t j = i; j <= LIM; j += i) {
                phi_arr[j] -= phi_arr[j] / i;
            }
        }
    }

    /* Allocate and count occurrences.  Index is uint64_t but fits
       into size_t because φ(i) ≤ i ≤ LIM. */
    occurrences = mem_new(seq_t, (LIM + 1));
    if (!occurrences) {
        free(phi_arr);
        return;
    }
    for (size_t i = 1; i <= LIM; ++i) {
        uint64_t v = phi_arr[i];
        if (v <= LIM) {
            occurrences[(size_t)v]++;   /* cast v to size_t for index */
        }
    }

    /* Clean up phi_arr as it is no longer needed. */
    free(phi_arr);
    phi_arr = NULL;

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();
    if (!initialized) return (seq_t)0;   /* fallback on OOM */

    /* Compute 8·prime(n).  The product fits in 64 bits for the
       required range. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t m = 8ull * p;
    if (m > LIM) return (seq_t)0;        /* safe guard */

    return occurrences[(size_t)m];
}
