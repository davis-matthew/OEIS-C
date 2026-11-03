/*
 * A086802  Triangle read by rows in which row n lists (prime(n)-prime(k))/2
 *          for 2 <= k <= n.
 * https://oeis.org/A086802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = (prime(n) – prime(k)) / 2, where k = 2 + ((n – 2) –
 *            T_{r‑1}) and the row r is the smallest integer with T_r > (n –
 *            2), T_j = j*(j-1)/2. The sequence is offset 2 (a(2) is the first
 *            listed term)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 92 (2026-11-01)
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

/* triangular number: T(r) = r*(r-1)/2 */
static inline uint64_t triangular(idx_t r)
{
    return (uint64_t)r * (r - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n < 2)            /* below the offset – value irrelevantly 0 */
        return (seq_t)0;

    idx_t idx = n - 2;    /* zero‑based index inside the flattened triangle */

    /* Find the row r that contains this index:
       smallest r with T(r) > idx.  Binary search. */
    idx_t low  = 2;
    idx_t high = idx + 2;   /* safe upper bound for the search */

    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) <= idx)
            low = mid + 1;
        else
            high = mid;
    }

    idx_t r = low;                      /* row containing the term */
    idx_t prev = triangular(r - 1);     /* terms before this row  */
    idx_t pos  = idx - prev;            /* position within the row (0‑based) */
    idx_t k    = pos + 2;                /* column index (k ≥ 2)    */

    seq_t prime_r = (seq_t)get_prime(r);
    seq_t prime_k = (seq_t)get_prime(k);

    return (seq_t)((prime_r - prime_k) / 2);
}
