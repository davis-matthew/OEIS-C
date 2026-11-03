/*
 * A178180  Numbers k such that prime(2k) mod prime(k) = prime(2k+2) mod
 *          prime(k+1).
 * https://oeis.org/A178180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178180: a(n) = the n‑th integer k (n ≥ 1) such that
   prime(2k) mod prime(k) == prime(2k+2) mod prime(k+1).
   The sequence starts a(1)=1.  The code evaluates the
   criterion directly for each candidate k; no table is stored. */

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

/* Return the n‑th term of A178180 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 – return 0 silently */

    idx_t k = 1;          /* candidate k */
    idx_t count = 0;      /* how many k satisfy the condition so far */

    for (;; ++k)
    {
        /* Obtain the required primes.  */
        uint32_t pk   = get_prime(k);
        uint32_t pk1  = get_prime(k + 1);
        uint32_t p2k  = get_prime(2 * k);
        uint32_t p2k2 = get_prime(2 * k + 2);

        /* Compare remainders.  */
        uint64_t r1 = (uint64_t)p2k  % pk;
        uint64_t r2 = (uint64_t)p2k2 % pk1;

        if (r1 == r2)
        {
            ++count;
            if (count == n)
                return (seq_t)k;   /* found the n‑th term */
        }
    }
}
