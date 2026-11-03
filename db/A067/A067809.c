/*
 * A067809  Composite n such that sigma(3*n) < prime(n).
 * https://oeis.org/A067809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A067809 – Composite n such that σ(3·n) < pₙ  
    where σ is the sum–of–divisors function and pₙ is the n‑th prime.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

/* Cache of terms already found, ordered by index. */
static seq_t *cached   = NULL;   /* array of a(k) values           */
static size_t cached_cap = 0;    /* allocated capacity of cached   */
static idx_t   cached_len = 0;   /* number of terms stored         */
static idx_t   last_tested = 1;  /* largest integer examined so far */

/* Return the n‑th term of A067809 (n >= 1). */
seq_t a(idx_t n)
{
    /* Grow cache until it contains at least n terms. */
    while (cached_len < n) {
        /* Next integer to examine. */
        last_tested++;

        /* Skip primes – we only need composites. */
        if (is_prime(last_tested))
            continue;

        /* Compute σ(3·last_tested) and compare with the n‑th prime. */
        idx_t product = (idx_t)3 * last_tested;
        seq_t sigma   = sigma_of(product);
        seq_t prime   = (seq_t)get_prime(last_tested);

        if (sigma < prime) {
            /* Add the new term to the cache. */
            if (cached_len == cached_cap) {
                /* Grow capacity, starting from 16 or doubling. */
                cached_cap = cached_cap ? cached_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cached, cached_cap);
                if (!tmp) abort();
                cached = tmp;
            }
            cached[cached_len++] = last_tested;
        }
    }

    return cached[n - 1];
}
