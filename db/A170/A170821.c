/*
 * A170821  Let p = n-th prime; a(n) = smallest k >= 0 such that 4k == 3 mod
 *          p.
 * https://oeis.org/A170821
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k ≥ 0 such that 4k ≡ 3 (mod prime(n)) *
 *            Algorithm: For an odd prime p, inv4 = 4⁻¹ (mod p) can be
 *            obtained * with the closed form inv4 = (p+1)/4 if p ≡ 3 (mod 4),
 *            * inv4 = (3p+1)/4 if p ≡ 1 (mod 4). * Then a(n) = (3 * inv4) mod
 *            p
 * Range:     u8 n <= 68, u16 n <= 8482, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 8623 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 2; return 0 otherwise. */
    if (n < 2)
        return 0;

    /* Retrieve the n-th prime (1‑based). */
    /* get_prime returns a uint32_t, but promote to 64‑bit for safety. */
    uint64_t p = (uint64_t)get_prime(n);

    /* Compute the modular inverse of 4 modulo p. */
    uint64_t inv4;
    if ((p & 3ULL) == 3ULL)          /* p ≡ 3 (mod 4) */
        inv4 = (p + 1ULL) / 4ULL;
    else                            /* p ≡ 1 (mod 4) */
        inv4 = (3ULL * p + 1ULL) / 4ULL;

    /* Compute a(n) = (3 * inv4) mod p. */
    uint64_t res = (3ULL * inv4) % p;

    return (seq_t)res;
}
