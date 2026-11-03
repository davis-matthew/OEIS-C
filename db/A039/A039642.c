/*
 * A039642  Number of steps to fixed point of "k -> k/2 or (k-1)/2 until
 *          result is prime", starting with prime(n)-1.
 * https://oeis.org/A039642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms (including the starting value) until
 *            reaching a prime, using the rule k -> k/2 if k is even,
 *            otherwise k -> (k-1)/2, starting from prime(n)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9598 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    if (x % 2 == 0)
        return x == 2;
    if (x % 3 == 0)
        return x == 3;
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0)
            return false;
        if (x % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* n-th prime (n >= 1) */
    uint64_t k = (uint64_t)p - 1;       /* starting value */
    seq_t steps = 1;                    /* count the starting term */

    for (;;) {
        if (k <= 1)                      /* reached 0 or 1 – stop */
            break;
        if (is_prime_uint64(k))          /* reached a prime – stop */
            break;

        /* compute the next value */
        uint64_t next = (k & 1ULL) ? ((k - 1ULL) >> 1) : (k >> 1);

        if (next == 0)                   /* avoid counting 0 */
            break;

        k = next;
        steps++;                          /* count this new term */
    }

    return steps;
}
