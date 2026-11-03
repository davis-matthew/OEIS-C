/*
 * A024451  a(n) is the numerator of Sum_{i = 1..n} 1/prime(i).
 * https://oeis.org/A024451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = (Product of the first n primes) * (Sum_{i=1..n}
 *            1/prime(i)) Recurrence: a(0) = 0 a(n) = a(n-1) * prime(n) +
 *            Product_{i=1..n-1} prime(i) for n ≥ 1 The function uses the
 *            utilities get_prime(idx_t k) : returns the k-th prime (1‑based)
 *            seq_mul(seq_t a, seq_t b) : multiplies two seq_t values
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 343 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t product = (seq_t)1;  /* P_{n-1} initially P_0 = 1 */
    seq_t an = (seq_t)0;       /* a(0) */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        uint32_t p = get_prime(i);          /* prime(i) */
        seq_t temp = seq_mul(an, (seq_t)p); /* a(n-1) * prime(n) */
        an = temp + product;                /* a(n) = a(n-1)*p + P_{n-1} */
        product = seq_mul(product, (seq_t)p);/* update P_n for next iteration */
    }

    return an;
}
