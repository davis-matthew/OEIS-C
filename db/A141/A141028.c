/*
 * A141028  A triangular sequence of the expansion of: (1-Prime[n])^n:
 *          t(n,m)=(-1)^m*Prime[n]^(n - m)*Binomial[n, m]; with Prime[0]=1
 *          defined to extend and lower the results.
 * https://oeis.org/A141028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   a(n) = (-1)^m * Prime[r]^(r-m) * C(r, m) where r is the unique
 *            non‑negative integer such that r*(r+1)/2 < n <= (r+1)*(r+2)/2,
 *            and m = n - r*(r+1)/2 - 1
 * Range:     i8 n <= 10, i16 n <= 15, i32 n <= 36, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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
    /* n >= 1 by definition of the sequence */
    /* Find the row index r */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < n)
        ++r;

    /* Position within the row (0‑based) */
    idx_t prev   = r * (r + 1) / 2;
    idx_t offset = n - prev;          /* 1 .. r+1 */
    idx_t m      = offset - 1;        /* 0 .. r */

    /* Retrieve the prime for this row; Prime[0] = 1 by definition */
    seq_t prime = (r == 0) ? (seq_t)1 : get_prime(r);

    /* Compute prime^(r-m) */
    seq_t exp  = (seq_t)(r - m);
    seq_t powp = ipow(prime, exp);

    /* Binomial coefficient C(r, m) */
    seq_t bin = binomial(r, m);

    /* Product */
    seq_t result = seq_mul(bin, powp);

    /* Apply the sign (-1)^m */
    if (m & 1)
        result = -result;

    return result;
}
