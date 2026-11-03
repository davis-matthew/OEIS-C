/*
 * A180267  Prime(2*n+1)!/(60*prime(2*n-1)!).
 * https://oeis.org/A180267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(2*n+1)! / (60 * prime(2*n-1)!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 11, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 767 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (prime(k) is the k‑th prime, primes indexed starting at 1) */

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
    /* a(0) is unused; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* indices of the required primes */
    idx_t idx_small = 2 * n - 1;   /* prime(2*n-1) */
    idx_t idx_large = 2 * n + 1;   /* prime(2*n+1) */

    /* obtain the prime values */
    seq_t small_prime = (seq_t)get_prime(idx_small);
    seq_t large_prime = (seq_t)get_prime(idx_large);

    /* compute product of integers in (small_prime, large_prime] and divide by 60 */
    seq_t result = (seq_t)1;

    /* factors of 60: 2^2 * 3 * 5 */
    seq_t factors[3] = { (seq_t)2, (seq_t)3, (seq_t)5 };
    int  counts  [3] = { 2,      1,      1      };

    /* loop over the integers between the two primes */
    idx_t start = (idx_t)small_prime + 1;
    idx_t end   = (idx_t)large_prime;

    for (idx_t i = start; i <= end; ++i)
    {
        result *= (seq_t)i;

        /* as soon as possible divide by the remaining factors of 60 */
        for (int j = 0; j < 3; ++j)
        {
            while (counts[j] > 0 && result % factors[j] == 0)
            {
                result /= factors[j];
                --counts[j];
            }
        }
    }

    /* all factors of 60 should have been removed */
    return result;
}
