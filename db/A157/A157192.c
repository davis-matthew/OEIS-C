/*
 * A157192  Triangle T(n, k) = 2^(n+k-2)*prime(k) + (n mod 2) if k <=
 *          floor(n/2) otherwise 2^(2*n-k-2)*prime(n-k) + (n mod 2), with T(n,
 *          0) = T(n, n) = 1, read by rows.
 * https://oeis.org/A157192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) , k = n -
 *            r(r+1)/2. The triangle entry is: T(r,0)=T(r,r)=1; otherwise
 *            2^(r+min(k,r-k)-2)*prime(min(k,r-k)) + (r mod 2)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 219, u64 n <= 839,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 82 terms through n = 1062 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

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
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n + 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* multiply with care: mid*(mid+1)/2 fits in idx_t */
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) low = mid + 1;
        else high = mid;
    }
    idx_t r = low - 1;
    idx_t k = n - r * (r + 1) / 2;

    if (k == 0 || k == r) return (seq_t)1;

    /* Reduce to the smaller side by symmetry T(r,k)=T(r,r-k) */
    idx_t ks = k;
    if (ks > r / 2) ks = r - ks;

    /* exponent for the power of two */
    int exp = (int)(r + ks - 2);          /* exp <= 2*r-2 <= 19998 for n<=10000 */

    /* 2^exp as a seq_t */
    seq_t two_pow = ipow_i32((seq_t)2, exp);

    /* prime(ks) as seq_t */
    seq_t prime_val = (seq_t)get_prime(ks);

    /* multiply and add the parity term */
    seq_t val = two_pow * prime_val;
    val = val + (seq_t)(r & 1);           /* add (r mod 2) */

    return val;
}
