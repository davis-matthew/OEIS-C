/*
 * A134296  Quotients A134295(p)/p = (1/p) * Sum_{k=1..p} ((p-k)!*(k-1)! -
 *          (-1)^k), where p = prime(n).
 * https://oeis.org/A134296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A134295(p) / p, where p = get_prime(n) and * A134295(p) =
 *            Σ_{k=1..p} ( (p-k)! * (k-1)! – (–1)^k ). * * We use the identity
 *            * A134295(p) = Σ_{i=0}^{p-1} ( i! * (p-1-i)! – (–1)^{i+1} ) *
 *            and compute the sum incrementally. The term i!*(p-1-i)! is *
 *            term_i = i! * (p-1-i)! * with recurrence * t...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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

/* Compute product of integers in [lo,hi] using binary splitting */
static seq_t prod_range(idx_t lo, idx_t hi)
{
    if (lo > hi)        return (seq_t)1;
    if (lo == hi)       return (seq_t)lo;
    if (hi - lo == 1)   return (seq_t)lo * (seq_t)hi;

    idx_t mid = lo + (hi - lo) / 2;
    seq_t left  = prod_range(lo,     mid);
    seq_t right = prod_range(mid + 1, hi);
    return left * right;
}

/* Forward declarations */
static seq_t prod_range(idx_t lo, idx_t hi);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Obtain the n-th prime */
    idx_t p = get_prime(n);

    /* Compute (p-1)! using binary splitting */
    seq_t fact = prod_range((idx_t)1, p - (idx_t)1);

    /* Sum over i = 0 .. p-1 of (i! * (p-1-i)! – (–1)^{i+1}) */
    seq_t sum = 0;
    seq_t term = fact;          /* term_0 = (p-1)! */
    for (idx_t i = 0; i < p; ++i) {
        if ( (i & 1) == 0 ) {   /* i even: add 1, <- because (–1)^{i+1} = –1 */
            sum += term + (seq_t)1;
        } else {                /* i odd: subtract 1, <– because (–1)^{i+1} = +1 */
            sum += term - (seq_t)1;
        }

        /* Prepare term_{i+1} for next iteration unless i == p-1 */
        if (i + (idx_t)1 < p) {
            /* term_{i+1} = term_i * (i+1) / (p-1-i)  (exact division) */
            idx_t numer = i + (idx_t)1;
            idx_t denom = p - (idx_t)1 - i;
            term = (term * (seq_t)numer) / (seq_t)denom;
        }
    }

    /* Divide by p to obtain a(n).  The result is integer by definition. */
    return sum / (seq_t)p;
}
