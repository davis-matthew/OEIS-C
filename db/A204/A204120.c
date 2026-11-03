/*
 * A204120  Symmetric matrix based on f(i,j) = gcd(prime(i+1),prime(j+1)), by
 *          antidiagonals.
 * https://oeis.org/A204120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5724, u16 n <= 65703, u32 n <= 65703, u64 n <= 65703,
 *            u128 n <= 65703, big any n
 * Validated: standard, pass, 113 terms through n = 9967 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204120:  a(n) = gcd(prime(i+1), prime(j+1)) for the (i,j) th
   cell of the infinite matrix, flattened along antidiagonals.
   Off‑diagonal entries are 1, diagonal entries are odd primes. */

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

/* Binary search for smallest t with t*(t+1)/2 >= n. */
static inline idx_t
find_antidiagonal(idx_t n)
{
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t sum = mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for offset 1, safety only */

    /* Identify antidiagonal t and position within it. */
    idx_t t        = find_antidiagonal(n);              /* 1‑based diagonal index */
    idx_t prev_sum = (t - 1) * t / 2;                   /* cumulative count up to t-1 */
    idx_t offset   = n - prev_sum - 1;                  /* 0‑based offset on diagonal */
    idx_t i        = offset + 1;                        /* row index (1‑based) */
    idx_t j        = t - offset;                        /* column index (1‑based) */

    if (i != j)
        return (seq_t)1;      /* off‑diagonal */

    /* Diagonal: return the (i+1)-th prime (prime(2)=3, prime(3)=5, …). */
    return (seq_t)get_prime(i + 1);
}
