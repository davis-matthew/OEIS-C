/*
 * A116563  a(n) is the genus of the modular curve X_0(p) for p = prime(n).
 * https://oeis.org/A116563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001617(prime(n)) = genus of the modular curve X0(p)
 *            where p = prime(n). Depending on p mod 12 the value is: - p ≡ 1
 *            (mod 12) : (p-13)/12 - p ≡ 5 (mod 12) : (p-5)/12 - p ≡ 7 (mod
 *            12) : (p-7)/12 - p ≡ 11 (mod 12): (p+1)/12 For p = 2,3 the
 *            result is 0
 * Range:     u8 n <= 438, u16 n <= 7058, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 66 terms through n = 8646 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute p mod 12. */
    uint32_t r = p % 12U;

    /* Apply the appropriate formula. */
    switch (r) {
        case 1:  return ((seq_t)p - 13) / 12;
        case 5:  return ((seq_t)p - 5)  / 12;
        case 7:  return ((seq_t)p - 7)  / 12;
        case 11: return ((seq_t)p + 1)  / 12;
        default: return 0;   /* primes 2,3,5,7,13,... handled above */
    }
}
