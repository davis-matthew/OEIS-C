/*
 * A090939  Least multiple of n == -1 (mod prime(n)).
 * https://oeis.org/A090939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ((prime(n)-1) * inv(n mod prime(n)) mod prime(n)),
 *            where inv is the modular inverse of n modulo the n-th prime
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
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

static void egcd(long long a, long long b, long long *x, long long *y)
{
    long long x0 = 1, y0 = 0;
    long long x1 = 0, y1 = 1;
    while (b != 0) {
        long long q = a / b;
        long long t = a - q * b; a = b; b = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
        t = y0 - q * y1; y0 = y1; y1 = t;
    }
    *x = x0;
    *y = y0;
}

seq_t a(idx_t n)
{
    uint64_t p = get_prime(n);            /* the n-th prime, p > n for all n >= 1 */
    long long n_ll = (long long)n;
    long long p_ll = (long long)p;

    /* modular inverse of n modulo p via extended Euclid */
    long long inv, dummy;
    egcd(n_ll, p_ll, &inv, &dummy);
    if (inv < 0) inv += p_ll;              /* make it positive */

    /* k = ((p-1) * inv) % p, but k = p if the residue is 0   */
    uint64_t k = ((uint64_t)(p - 1) * (uint64_t)inv) % p;
    if (k == 0) k = p;

    return (seq_t)n * (seq_t)k;             /* result fits in seq_t for n <= 10000 */
}
