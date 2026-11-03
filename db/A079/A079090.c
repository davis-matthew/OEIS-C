/*
 * A079090  Sum of distinct prime factors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of
 *            ((prime(n)+1)*(prime(n+1)+1))/4
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 7352, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9906 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* Retrieve the n-th and (n+1)-th primes */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Compute (p_n+1)*(p_{n+1}+1)/4 as a seq_t value */
    seq_t prod = (seq_t)(p1 + 1) * (seq_t)(p2 + 1);
    seq_t x = prod / 4;

    /* Factor x and sum its distinct prime divisors */
    seq_t temp = x;
    seq_t sum = 0;
    for (seq_t d = 2; d * d <= temp; ++d)
    {
        if (temp % d == 0)
        {
            sum += d;          /* add this distinct prime factor */
            while (temp % d == 0)
                temp /= d;     /* strip all powers of d */
        }
    }
    if (temp > 1)
        sum += temp;           /* remaining prime factor */

    return sum;
}
