/*
 * A152755  Triangle read by rows: row n gives the vector b[n] where b[0] =
 *          {-1}; b[1] = {1, -1}; b[n] = prime[n + 2 ], -prime[n + 2] + n - 1,
 *          -1,-1, ..., -1.
 * https://oeis.org/A152755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 464, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Validated: standard, pass, 82 terms through n = 1325 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152755
   Triangle read by rows: row n gives b[n] where
   b[0]   = {-1};
   b[1]   = { 1, -1};
   b[n]   = { prime[n+2],   -prime[n+2] + n - 1,   -1, -1, ..., -1 }   (n ≥ 2)
   Offset: 0
   Implementation uses integer binary search to locate the row,
   and get_prime(k) to obtain the (k)-th prime (1‑based). */

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
    /* --- locate the row r containing index n --- */
    idx_t low = 0;
    idx_t high = 1;

    /* increase high until triangular(high) > n */
    while ( (unsigned __int128)high * (high + 1) / 2 <= n )
    {
        if (high > (idx_t)0x8000000000000000ULL) /* avoid overflow */
            break;
        high <<= 1;
    }

    /* binary search for largest r with r*(r+1)/2 <= n */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;   /* r = row number */

    /* position of n within its row */
    idx_t pos = n - (unsigned __int128)r * (r + 1) / 2;

    /* --- compute the value --- */
    if (r == 0)
    {
        /* row 0 */
        return (seq_t)-1;
    }
    else if (r == 1)
    {
        /* row 1: {1, -1} */
        return (pos == 0) ? (seq_t)1 : (seq_t)-1;
    }
    else
    {
        /* row r ≥ 2 */
        seq_t p = (seq_t)get_prime(r + 2);   /* first element is prime[r+2] */

        if (pos == 0)
            return p;
        else if (pos == 1)
        {
            seq_t r_seq = (seq_t)r;
            return -p + r_seq - 1;
        }
        else
        {
            return (seq_t)-1;
        }
    }
}
