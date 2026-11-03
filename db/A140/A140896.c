/*
 * A140896  An even-powered type Binet p-adic triangular sequence: t(n,m)=(((
 *          1 + sqrt(prime(n))))^(2*m) + (( 1 - sqrt(prime(n))))^(2*m))/2.
 * https://oeis.org/A140896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   a(n) = t(r,m) where r is the row containing the nth term and m
 *            is the term's position in that row: t(r,m) = ((1 +
 *            sqrt(prime(r)))^(2m) + (1 - sqrt(prime(r)))^(2m)) / 2 (primes
 *            are indexed starting at 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* n < 1 is outside the OEIS offset; return 0 for safety */
    if (n < 1) return 0;

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;           /* sum_{i=1..r} i */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Position m within row r (1‑based) */
    idx_t prev_cumulative = cumulative - r; /* (r-1)*r/2 */
    idx_t m = n - prev_cumulative;          /* 1 <= m <= r */

    /* Prime of the row */
    uint32_t prime32 = get_prime(r);
    seq_t p = (seq_t)prime32;

    /* Compute Lucas number L_{2m} with recurrence:
         L_0 = 2, L_1 = 2
         L_k = 2*L_{k-1} + (p-1)*L_{k-2}
       Then t(r,m) = L_{2m} / 2  */
    seq_t L_prev2 = (seq_t)2;   /* L0 */
    seq_t L_prev1 = (seq_t)2;   /* L1 */

    idx_t target = 2 * m;       /* we need L_{2m} */
    for (idx_t i = 2; i <= target; ++i) {
        seq_t L = (seq_t)2 * L_prev1 + (p - (seq_t)1) * L_prev2;
        L_prev2 = L_prev1;
        L_prev1 = L;
    }

    return L_prev1 / (seq_t)2;  /* divide by 2 to obtain the term */
}
