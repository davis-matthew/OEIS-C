/*
 * A207409  Triangular array: T(n,k) = prime(n) mod prime(k), 1 <= k < n.
 * https://oeis.org/A207409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(r) mod prime(k) where r and k are derived from the
 *            triangular array T(r,k) = prime(r) mod prime(k) (1 ≤ k < r) by
 *            enumerating the array row‑by‑row starting at n = 2. The integer
 *            m = n-2 is zero‑based; find the smallest r such that r(r-1)/2 >
 *            m, let k = m - (r-2)(r-1)/2 + 1, and retur...
 * Range:     u8 n <= 1870, u16 n <= 10012, u32 n <= 10012, u64 n <= 10012,
 *            u128 n <= 10012, big any n
 * Validated: standard, pass, 108 terms through n = 9951 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n < 2)                         /* no defined terms before offset */
        return 0;

    idx_t m = n - 2;                    /* zero‑based array index   */
    idx_t r = 2;                        /* start with first row (r=2) */
    while ((r - 1) * r / 2 <= m)        /* find row containing m   */
        r++;

    /* previous cumulative entries up to row r-1 */
    idx_t prev = (r - 2) * (r - 1) / 2;
    idx_t k = m - prev + 1;             /* column index (1‑based)   */

    /* compute the required entry */
    seq_t prime_r = (seq_t)get_prime(r);
    seq_t prime_k = (seq_t)get_prime(k);

    return prime_r % prime_k;
}
