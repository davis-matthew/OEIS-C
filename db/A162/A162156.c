/*
 * A162156  Table which contains in row n the mapping of the n-th block of 4
 *          primes to 4 integers.
 * https://oeis.org/A162156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 33, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162156: 4‑by‑4 product combinations of consecutive primes.
   a(n) is the nth entry in row‑major order of matrix
   T(n,1), T(n,2), T(n,3), T(n,4).

   Terms are given by:
     c = prime(4n-3),  b = prime(4n-2),  m = prime(4n-1),  r = prime(4n)
   T(n,1) = b*m - c*r
   T(n,2) = c*m + b*r
   T(n,3) = b*m + c*r
   T(n,4) = c*m - b*r
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

seq_t a(idx_t n)
{
    if (n < 1) return 0;               // undefined for n = 0, but we return 0
    idx_t row = (n - 1) / 4 + 1;        // 1‑based row index
    idx_t i   = (n - 1) % 4;            // 0‑based column               

    uint32_t p1 = get_prime(4 * row - 3);   // c
    uint32_t p2 = get_prime(4 * row - 2);   // b
    uint32_t p3 = get_prime(4 * row - 1);   // m
    uint32_t p4 = get_prime(4 * row);       // r

    switch (i) {
        case 0: // T(n,1)
            return (seq_t)p2 * p3 - (seq_t)p1 * p4;
        case 1: // T(n,2)
            return (seq_t)p1 * p3 + (seq_t)p2 * p4;
        case 2: // T(n,3)
            return (seq_t)p2 * p3 + (seq_t)p1 * p4;
        case 3: // T(n,4)
            return (seq_t)p1 * p3 - (seq_t)p2 * p4;
        default:
            return 0; // unreachable
    }
}
