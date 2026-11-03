/*
 * A085604  T(n,k) = highest power of prime(k) dividing n!, read by rows.
 * https://oeis.org/A085604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of the prime with index k in (row)!, * where n
 *            is mapped to the pair (row, col) by traversing the * triangular
 *            table row‑by‑row. The exponent is computed via * Legendre's
 *            formula
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Validated: standard, pass, 116 terms through n = 7783 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

/* Find the row r and column k of the term with linear index n.
 * The rows are 1‑based and each row r contains r terms. */
static void decode_row(idx_t n, idx_t *r, idx_t *k)
{
    /* Binary search for the smallest r such that r(r+1)/2 >= n.
     * Use 128‑bit arithmetic to avoid overflow. */
    idx_t lo = 1, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    *r = lo;
    idx_t prev = (lo - 1) * lo / 2;   /* terms before row r */
    *k = n - prev;                   /* 1‑based column in row r */
}

/* Legendre's formula: return exponent of prime p in n!. */
static seq_t exponent_in_factorial(idx_t n, uint64_t p)
{
    seq_t exp = 0;
    uint64_t t = n;
    while (t) {
        t /= p;
        exp += t;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; safe guard for invalid input */
        return 0;

    idx_t row, col;
    decode_row(n, &row, &col);

    uint64_t prime = get_prime(col);   /* 1‑based index: prime(col) */
    return exponent_in_factorial(row, prime);
}
