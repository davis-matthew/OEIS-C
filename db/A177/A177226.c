/*
 * A177226  Triangle read by rows: T(n, k) = 2^(prime(n) - prime(k)) mod
 *          prime(n), 1 <= k <= n.
 * https://oeis.org/A177226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 107 terms through n = 1218 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A177226: triangle read by rows
 * a(n) = 2^(prime(r) - prime(k)) mod prime(r)
 * where r = smallest integer such that r(r+1)/2 >= n
 * and k = n - (r-1)r/2  (the column index in row r)
 * 
 * The values are generated on demand; no hard‑coded table is used.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n ) r++;

    /* compute column index k */
    idx_t offset = 1 + (r - 1) * r / 2;   /* first index in this row */
    idx_t k = n - offset + 1;             /* 1‑based column */

    /* obtain the r‑th and k‑th primes (uint32_t fits all required indices) */
    uint32_t p_r = get_prime(r);
    uint32_t p_k = get_prime(k);

    /* exponent for the power of two */
    uint64_t exp = (uint64_t)p_r - (uint64_t)p_k;

    /* compute the triangle entry */
    return powmod((seq_t)2, exp, (seq_t)p_r);
}
