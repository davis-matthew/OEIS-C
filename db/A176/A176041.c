/*
 * A176041  Triangle read by rows: R(n, k) = 2^(2n - 2) mod prime(2k),
 *          1<=k<=n.
 * https://oeis.org/A176041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(2r-2) mod prime(2k), where (r,k) is the position of
 *            the n‑th term in the triangle read row by row
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/powmod.h"

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
    /* Find row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n */
    idx_t low = 1, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum < n) {
            low = mid + 1;
        } else {
            r = mid;
            high = mid - 1;
        }
    }

    /* Compute column k within row r */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;            /* 1 <= k <= r */

    /* Evaluate 2^(2r-2) mod prime(2k) */
    idx_t prime_index = 2 * k;            /* 2k-th prime */
    seq_t mod = (seq_t)get_prime(prime_index);
    idx_t exp = 2 * r - 2;
    return powmod((seq_t)2, exp, mod);
}
