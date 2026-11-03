/*
 * A126997  a(n) = binomial(prime(4+n), prime(4)).
 * https://oeis.org/A126997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(prime(4 + n), prime(4))
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 19, u64 n <= 289, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 975 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The 4th prime is 7.  The binomial coefficient is computed
   multiplicatively to avoid overflow when the intermediate values
   would exceed the width of seq_t.  For seq_t types that are
   arbitrary‑precision this works unchanged; for fixed‑width
   types the computation stays within the supported range for all
   terms that fit. */

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

static inline seq_t binomial_choose7(idx_t n)
{
    /* Computes C(n,7) = n*(n-1)*...*(n-6)/7!   */
    seq_t r = 1;
    for (idx_t i = 1; i <= 7; ++i)
    {
        /* Multiply first, then divide.  The intermediate product
           may exceed seq_t when seq_t is 64‑bit, so we cast to a
           wider type for the multiplication.  For arbitrary‑precision
           seq_t this casting has no effect. */
        unsigned __int128 prod = (unsigned __int128)r * (n - (7 - i));
        r = (seq_t)(prod / i);
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Obtain the (4+n)th prime.  get_prime returns a 32‑bit integer,
       which is safely convertible to idx_t. */
    idx_t pk = (idx_t)get_prime(4 + n);

    /* Since the fourth prime is fixed (7), we can compute the
       binomial coefficient directly. */
    return binomial_choose7(pk);
}
