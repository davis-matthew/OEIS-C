/*
 * A240673  Triangle read by rows: T(n, k) = v(n, k)*((1/v(n, k)) mod
 *          prime(k)), where v(n, k) = (Product_{j=1..n} prime(j))/prime(k), n
 *          >= 1, 1 <= k <= n.
 * https://oeis.org/A240673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row containing the n-th element
 *            (r(r+1)/2 ≥ n) and k = n - r(r-1)/2 is the column in that row.
 *            T(r,k) = v(r,k) * inv(v(r,k) mod prime(k), prime(k)), with
 *            v(r,k) = ∏_{j=1..r, j≠k} prime(j)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 50, u64 n <= 121,
 *            u128 n <= 351, big any n
 * Validated: standard, pass, 61 terms through n = 9057 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Offset starts at 1. For n < 1 the behavior is undefined. */
    if (n < 1) return 0;

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column k in the row. */
    idx_t k = n - (r - 1) * r / 2;   /* 1 ≤ k ≤ r */

    /* Compute v(r,k) = ∏_{j=1..r, j≠k} prime(j). */
    seq_t v = 1;
    for (idx_t j = 1; j <= r; ++j) {
        if (j == k) continue;
        uint32_t pj = get_prime(j);
        v = seq_mul(v, (seq_t)pj);
    }

    /* Modular inverse of v modulo prime(k). */
    uint32_t pk = get_prime(k);
    seq_t v_mod = v % pk;                     /* v mod pk  */
    seq_t inv   = powmod(v_mod, (uint64_t)pk - 2, (seq_t)pk);

    /* Return T(r,k) = v * inv. */
    return seq_mul(v, inv);
}
