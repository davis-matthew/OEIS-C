/*
 * A173556  Triangle T(n,m) = (prime(n)-1)*(prime(m)- 1) mod 8 read by rows.
 * https://oeis.org/A173556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (prime(i) - 1)*(prime(j) - 1) mod 8, where the pair (i,j)
 *            is determined by the position of n in the row‑wise traversal of
 *            the triangle T(r,c) = (prime(r) - 1)*(prime(c) - 1) mod 8
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 106 terms through n = 106 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flags and helpers ---------------------------------------------------- */
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

/* Limited integer arithmetic with 128‑bit for safety ----------------- */
/* Return the r‑th triangular number: r*(r+1)/2, safe up to 64‑bit.     */
static inline idx_t triangular(idx_t r)
{
    __uint128_t t = (__uint128_t)r * (r + 1) / 2;
    return (idx_t)t;
}

/* Return the smallest r such that triangular(r) >= n.  Binary search. */
static inline idx_t row_for_index(idx_t n)
{
    idx_t lo = 1, hi = n * 2;          /* hi is a safe upper bound */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (triangular(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main sequence generator ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Treat bogus input never encountered in the problem domain */
    if (n < 1) return (seq_t)0;

    /* Find the row r and column c in the triangle */
    idx_t r = row_for_index(n);
    idx_t c = n - triangular(r - 1);   /* 1‑based column */

    /* Retrieve the relevant primes */
    uint32_t pr = get_prime(r);
    uint32_t pc = get_prime(c);

    /* Compute (prime-1) modulo 8 for each factor */
    uint32_t a_r = (pr - 1) & 7u;
    uint32_t a_c = (pc - 1) & 7u;

    /* Product modulo 8 */
    uint32_t prod = (a_r * a_c) & 7u;

    return (seq_t)prod;
}
