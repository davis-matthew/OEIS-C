/*
 * A237643  Least positive integer m such that {A000720(k*n): k = 1, ..., m}
 *          contains a complete system of residues modulo n, or 0 if such a
 *          number m does not exist.
 * https://oeis.org/A237643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 76 terms through n = 100 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237643
   a(n) = least positive integer m such that the set { pi(k*n) : 1 ≤ k ≤ m }
   contains a complete system of residues modulo n; a(n) = 0 if no such m exists.
   (pi = prime counting function.  The function _compute_pi_ is provided in
   "../utils/compute_pi.h".  The function _get_prime_ returns the nth prime
   and is provided in "../utils/get_prime.h".) */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* The definition is only meaningful for n ≥ 1. */
    if (n == 0) return 0;

    /* Allocate a boolean table of size n to mark which residues have
       been seen.  `calloc` returns `void*`; cast to `bool*` to satisfy
       the C++ compiler. */
    size_t sz = (size_t)n;
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) return 0;          /* allocation failure → return 0 */

    size_t remaining = sz;        /* how many residues are still missing */

    /* Upper bound for the search: 2 * prime(n).  The conjecture states
       that a(n) ≤ 2*prime(n) for all n > 0. */
    idx_t prime_n = get_prime(n); /* nth prime */
    idx_t limit = prime_n * 2;

    for (idx_t k = 1; k <= limit; ++k)
    {
        /* Compute pi(k*n).  The product fits in idx_t for the ranges
           considered here (n ≤ 10 000, k ≤ 2*prime(n)). */
        seq_t pi_val = prime_pi(k * n);

        /* Reduce modulo n.  The % operator works between seq_t and
           seq_t, so convert n to seq_t first. */
        seq_t residue_seq = pi_val % (seq_t)n;
        idx_t residue = (idx_t)residue_seq;  /* now a valid index < n */

        if (!seen[residue])
        {
            seen[residue] = true;
            if (--remaining == 0)
            {
                free(seen);
                return (seq_t)k;   /* all residues are present */
            }
        }
    }

    free(seen);
    return 0;  /* no m found within the conjectured bound */
}
