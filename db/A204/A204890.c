/*
 * A204890  Ordered differences of primes.
 * https://oeis.org/A204890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For n≥1, order all ordered pairs (i,j) with 1≤i<j by increasing j,
   then increasing i. The nth term a(n) is the prime difference
   p_j − p_i, where j is the smallest integer with j(j−1)/2 ≥ n
   and i = n − (j−2)(j−1)/2. */
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
    if (n == 0) return (seq_t)0;

    idx_t j = 2;
    idx_t cum = 1;                       /* (j-1)*j/2 for j=2 */
    while (cum < n) {
        ++j;
        cum += j - 1;                    /* add new rows of differences */
    }

    /* cumulative count up to row j, and previous row count */
    idx_t prev = cum - (j - 1);          /* (j-2)*(j-1)/2 */
    idx_t i = n - prev;                  /* 1‑based index in the primes list */

    return (seq_t)get_prime(j) - (seq_t)get_prime(i);
}
