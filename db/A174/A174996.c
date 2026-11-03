/*
 * A174996  Triangle read by rows: T(n,k) = (prime(n)-1) mod prime(k).
 * https://oeis.org/A174996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 121 terms through n = 1275 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS A174996: triangle read by rows
   T(m,k) = (prime(m)-1) mod prime(k), where m≥1, 1≤k≤m.
   The sequence a(n) enumerates the triangle row‑by‑row,
   so a(n) is T(m,k) with m the row containing the n‑th term
   and k the position within that row. */
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

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* offset is 1, below values undefined */

    /* Find the row m such that m*(m+1)/2 ≥ n.
       The cumulative count up to row m-1 is (m-1)*m/2. */
    idx_t m = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++m;
        cumulative += m;
    }

    /* Column index k within row m */
    idx_t k = n - (cumulative - m);   /* 1 ≤ k ≤ m */

    /* Compute (prime(m)-1) mod prime(k) */
    uint32_t pm = get_prime(m);
    uint32_t pk = get_prime(k);

    seq_t result = (seq_t)(pm - 1) % (seq_t)pk;
    return result;
}
