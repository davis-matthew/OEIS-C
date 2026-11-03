/*
 * A082786  Triangle, read by rows, of exponents of primes in canonical prime
 *          factorization of n: T(n,k) = greatest number such that
 *          prime(k)^T(n,k) divides n, 1 <= k <= n.
 * https://oeis.org/A082786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of prime(k) in the prime factorization of row r,
 *            where r is the smallest integer with r(r+1)/2 >= n and k = n −
 *            r(r−1)/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* Find the row r: smallest integer with r(r+1)/2 >= n. */
    idx_t low = 1;
    idx_t high = n;               /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column k within the row (1‑based). */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev);  /* 1 <= k <= r */

    /* Exponent of prime(k) in the factorization of r. */
    uint32_t p = get_prime(k);    /* kth prime: 2,3,5,7,... */
    seq_t exp = 0;
    idx_t temp = r;
    while (temp % p == 0) {
        ++exp;
        temp /= p;
    }
    return exp;
}
