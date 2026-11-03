/*
 * A199263  Let CardE(K) the number of elements (x, y) of the finite group
 *          defined by the elliptic curve y^2 = x^3 + x + 1 (mod p) including
 *          the point at infinity; a(n) is the difference between 2*sqrt(p)
 *          and |cardE(K) - (p + 1)|.
 * https://oeis.org/A199263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2*sqrt(p) - |#E(F_p) - (p+1)| ), where p =
 *            prime(n) and #E(F_p) is the number of points (x,y) on y^2 = x^3
 *            + x + 1 mod p plus the point at infinity
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
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
    uint64_t p = get_prime(n);
    /* Count points on the elliptic curve over F_p. */
    uint64_t count = 0;          /* sum of the RHS solutions for each x */
    if (p == 2)                  /* handle characteristic 2 separately */
    {
        for (uint64_t x = 0; x < 2; ++x)
        {
            uint64_t f = (x * x * x + x + 1) & 1;  /* modulo 2 */
            if (f == 0 || f == 1)
                ++count;   /* exactly one y in F_2 */
        }
    }
    else
    {
        /* Allocate a temporary table to mark quadratic residues. */
        unsigned char *square = (unsigned char*)calloc(p, 1);
        if (!square) return 0; /* allocation failed, return 0 as safe default */

        /* Mark squares. */
        for (uint64_t y = 0; y < p; ++y)
            square[(uint64_t)(y * y) % p] = 1;

        for (uint64_t x = 0; x < p; ++x)
        {
            uint64_t f = ( (x * x % p) * x % p + x + 1 ) % p;
            if (f == 0)
                ++count;                 /* one y = 0 */
            else if (square[f])
                count += 2;              /* two y solutions */
            /* else: no y */
        }
        free(square);
    }
    /* Add the point at infinity. */
    count += 1;

    /* Compute the quantity a(n). */
    double val = 2.0 * sqrt((double)p);
    int64_t diff = (int64_t)floor(val - llabs((int64_t)count - (int64_t)(p + 1)));
    if (diff < 0) diff = 0;   /* safety; diff should not be negative */

    return (seq_t)diff;
}
