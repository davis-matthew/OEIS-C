/*
 * A147815  Triangle t(n, k) = k*n*(prime(n+2) - 2*prime(n+1) + prime(n)) +
 *          prime(n), 0 <= k <= n = 1, 2, 3, ...
 * https://oeis.org/A147815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   a(n) = t(N,K) where N is the first row with * N(N+3)/2 ≥ n, * K
 *            = n − (N−1)(N+2)/2 − 1 (0‑based column inside the row), * t(N,K)
 *            = K*N*(prime(N+2)−2*prime(N+1)+prime(N)) + prime(N)
 * Range:     i8 n <= 42, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
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
    if (n == 0)  /* undefined for n < 1; return 0 for safety */
        return 0;

    /* Find the smallest N such that N(N+3)/2 >= n.
     * Binary search between 1 and n (N cannot exceed n). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Use 128‑bit arithmetic to avoid overflow of mid*(mid+3)  */
        unsigned __int128 val = (unsigned __int128)mid * (mid + 3) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t N = low;                      /* required row */

    /* Elements before row N: sum_{i=1}^{N-1} (i+1) = (N-1)(N+2)/2 */
    unsigned __int128 sumPrev128 = (unsigned __int128)(N - 1) * (N + 2) / 2;
    idx_t  sumPrev = (idx_t)sumPrev128;        /* fits in idx_t */

    /* 0‑based column within the row */
    idx_t K = n - sumPrev - 1;

    /* Retrieve the needed primes */
    uint32_t pN  = get_prime(N);      /* prime(N)   */
    uint32_t pN1 = get_prime(N + 1);  /* prime(N+1) */
    uint32_t pN2 = get_prime(N + 2);  /* prime(N+2) */

    /* Compute the triangle entry */
    seq_t diff  = (seq_t)pN2 - (seq_t)2 * (seq_t)pN1 + (seq_t)pN;
    seq_t result = (seq_t)K * (seq_t)N * diff + (seq_t)pN;
    return result;
}
