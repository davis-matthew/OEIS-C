/*
 * A141658  A triangle of coefficients representing an Integer Irrational
 *          rotation of the primes: t(n,m)=Floor[Re[Exp[I*2*Pi*Mod[m*2*Pi,
 *          1]/(n + 2)]*Prime[n + 1]]]. ( Correction with right numbers).
 * https://oeis.org/A141658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   t(n,m) = Floor[Re[Exp[I*2*Pi*Mod[m*2*Pi, 1]/(n + 2)]*Prime[n +
 *            1]]] a(n) is the flattened triangle: a(n) = t(r-1, m-1) where r
 *            is the row containing index n and m its column index (both
 *            1‑based)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
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
    /* Natural numbers start at 1. n < 1 is not used. */
    idx_t row = 0;
    idx_t len = 1;           /* cumulative number of elements up to current row */
    /* Find the row that contains the index n. */
    while (n > len) {
        row++;
        len += (row + 1);    /* each new row adds row+1 elements */
    }

    /* Elements before this row */
    idx_t before = row * (row + 1) / 2;
    idx_t idx_in_row = n - before - 1;   /* 0‑based column index within the row */

    /* Retrieve the required prime.  get_prime is 1‑based. */
    uint32_t prime = get_prime(row + 1);

    /* Compute the angle for the cosine factor. */
    const double PI = acos(-1.0);
    double fractional = fmod((double)idx_in_row * 2.0 * PI, 1.0);      /* Mod[m*2π,1] */
    double angle = 2.0 * PI * fractional / (double)(row + 2);         /* /(n+2) */

    /* Evaluate floor[prime * cos(angle)] */
    double prod = prime * cos(angle);
    long long val = (long long)floor(prod);

    return (seq_t)val;
}
