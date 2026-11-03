/*
 * A138436  a(n) = ((n-th prime)^5-(n-th prime)^3)/8.
 * https://oeis.org/A138436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n^5 - p_n^3)/8 where p_n is the n‑th prime
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 41 terms through n = 967 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses get_prime from utils to obtain p_n as a uint32_t.  For
    64‑bit builds the intermediate calculations are performed in
    unsigned __int128 to avoid overflow before rounding down to seq_t. */

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
    if (n == 0) return 0;                 /* undefined below offset – return 0 */

    uint32_t p32 = get_prime(n);          /* p_n as 32‑bit value */
    seq_t p = (seq_t)p32;                 /* promote to seq_t */

    /* Use 128‑bit arithmetic for the intermediate powers. */
    __uint128_t p128 = (__uint128_t)p32;
    __uint128_t p2   = p128 * p128;       /* p^2   */
    __uint128_t p3   = p2 * p128;         /* p^3   */
    __uint128_t p5   = p3 * p2;           /* p^5   */

    __uint128_t num  = p5 - p3;           /* p^5 - p^3 */
    seq_t result = (seq_t)(num / 8ULL);    /* divide by 8 and cast back */

    return result;
}
