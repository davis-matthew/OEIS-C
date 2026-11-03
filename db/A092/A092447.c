/*
 * A092447  Concatenate odd primes in decreasing order.
 * https://oeis.org/A092447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate of the first n odd primes in decreasing order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities provided by the build environment */
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Count decimal digits of a 32‑bit unsigned integer. */
static inline size_t digit_count_uint32(uint32_t x)
{
    size_t d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0) return result;

    /*  get_prime(1) == 2, so for n odd primes we start from index n+1 (the nth odd prime) */
    for (idx_t k = n + 1; k >= 2; --k)
    {
        uint32_t p = get_prime(k);            /* kth prime (primes[1] == 2) */
        size_t d = digit_count_uint32(p);     /* number of decimal digits of p */
        result = result * pow10((idx_t)d) + (seq_t)p;
    }
    return result;
}
