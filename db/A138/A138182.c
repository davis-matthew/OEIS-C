/*
 * A138182  Smallest summand in the Zeckendorf representation of the n-th
 *          prime.
 * https://oeis.org/A138182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A139764(A000040(n))
 * Range:     u8 n <= 50, u16 n <= 5473, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9652 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest Fibonacci summand in the Zeckendorf
 * representation of the n-th prime (1‑based). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* Compute the n-th prime (n >= 1) using the provided helper. */
static inline uint32_t nth_prime(idx_t n)
{
    /* get_prime() is defined in utils/get_prime.h and returns the
     * n-th prime as a uint32_t.  It already handles the 1‑based index. */
    return get_prime(n);
}

/* Return the smallest Zeckendorf summand for the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safeguard for out‑of‑range call */

    const uint32_t prime_uint = nth_prime(n);
    const seq_t prime = (seq_t)prime_uint;

    /* Generate Fibonacci numbers up to the prime.
       A small fixed buffer is sufficient – 64 elements covers all
       Fibonacci numbers less than 2^32. */
    seq_t fibs[64];
    size_t cnt = 0;
    fibs[cnt++] = (seq_t)1;
    fibs[cnt++] = (seq_t)2;

    while (true) {
        const seq_t next = fibs[cnt - 2] + fibs[cnt - 1];
        if (next > prime) break;
        fibs[cnt++] = next;
    }

    /* Greedy Zeckendorf decomposition to find the smallest summand. */
    seq_t rem = prime;
    seq_t min_summand = 0;          /* will hold the last (i.e. smallest) summand */

    for (size_t i = cnt; i > 0; --i) {
        const seq_t f = fibs[i - 1];
        if (f <= rem) {
            rem = rem - f;
            min_summand = f;        /* last chosen summand is the smallest */
            if (rem == 0) break;    /* complete decomposition */
        }
    }

    return min_summand;
}
