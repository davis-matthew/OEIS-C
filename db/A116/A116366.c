/*
 * A116366  Triangle read by rows: even numbers as sums of two odd primes.
 * https://oeis.org/A116366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = prime(n+1)+prime(k+1) (OEIS A116366). a(n) is the * nth
 *            term of the triangle read by rows
 * Range:     u8 n <= 494, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 94 terms through n = 4792 (2026-11-01)
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
    /* The cumulative number of terms up to row r is r*(r+1)/2.  Find
     * the smallest r such that r*(r+1)/2 >= n.  Then the position
     * inside that row is k = n - (r-1)*r/2.  Finally, a(n) = 
     * prime(r+1) + prime(k+1).  All operations use idx_t and seq_t. */
    if (n == 0)
        return (seq_t)0;          /* Undefined for offset 0, but handled. */

    idx_t row = 1;     /* Current row index (1‑based)       */
    idx_t cum = 1;     /* Cumulative terms up to current row */

    while (cum < n) {
        row++;
        cum += row;
    }

    /* k is 1‑based position within the row. */
    idx_t k = n - (cum - row);

    seq_t prime_row = (seq_t)get_prime(row + 1);
    seq_t prime_k   = (seq_t)get_prime(k + 1);

    return prime_row + prime_k;
}
