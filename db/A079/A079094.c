/*
 * A079094  Arithmetic derivative of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Arithmetic derivative of (prime(n)+1)*(prime(n+1)+1)/4 a(n) =
 *            A003415( (prime(n)+1)*(prime(n+1)+1)/4 )
 * Range:     u8 n <= 6, u16 n <= 69, u32 n <= 5660, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9940 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

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

static seq_t arithmetic_derivative(uint64_t m)
{
    seq_t sum = 0;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            sum += (seq_t)(m / p) * (seq_t)e;
        }
    }
    if (tmp > 1) {    /* remaining prime factor */
        sum += (seq_t)(m / tmp);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* get consecutive primes */
    uint32_t pn  = get_prime(n);
    uint32_t pn1 = get_prime(n + 1);

    /* compute m = (p_n+1)*(p_{n+1}+1)/4 as a seq_t */
    seq_t prod = (seq_t)(pn + 1) * (seq_t)(pn1 + 1);
    prod /= (seq_t)4;             /* exact division */

    /* convert to uint64_t for factorisation (value fits in 64 bits) */
    uint64_t m = (uint64_t)prod;
    return arithmetic_derivative(m);
}
