/*
 * A155474  Odd nonprimes n with distinct smallest digits of n and n-th prime.
 * https://oeis.org/A155474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth odd nonprime number n such that the smallest
 *            decimal digit of n and the nth prime have different smallest
 *            digits
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
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

/* Smallest decimal digit of x.  Works for any unsigned seq_t. */
static inline seq_t smallest_digit(seq_t x)
{
    seq_t min = (seq_t)9;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        if (d < min) min = d;
        x /= (seq_t)10;
    }
    return min;
}

/* Cache for the computed terms.  terms[0] holds a(1). */
static seq_t *terms = NULL;
static size_t terms_size = 0;   /* number of terms stored so far */
static size_t terms_cap  = 0;   /* allocated capacity (in elements) */

/* Last integer that was fully processed (odd nonprime). */
static idx_t last_checked = 0;

/* Return the n‑th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard anyway */

    /* Grow the cache until it contains at least n terms. */
    while (terms_size < (size_t)n) {
        /* Find the next odd integer. */
        do { ++last_checked; } while ((last_checked & 1) == 0); /* skip evens */

        /* Skip primes – we need odd non‑primes only. */
        if (is_prime(last_checked)) continue;

        /* Compare smallest digits of the candidate and the n‑th prime. */
        seq_t d1 = smallest_digit((seq_t)last_checked);
        seq_t p  = (seq_t)get_prime(last_checked);  /* p = prime(last_checked) */
        seq_t d2 = smallest_digit(p);

        if (d1 != d2) {
            /* Store this term.  Grow the array if required. */
            if (terms_size + 1 > terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                if (!tmp) abort();   /* memory allocation failure */
                terms = tmp;
            }
            terms[terms_size++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
