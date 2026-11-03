/*
 * A116151  a(n) = smallest positive integer x satisfying the system of
 *          congruences { x == 1 (mod 2), x == 2 (mod 3), x == 3 (mod 5), x ==
 *          5 (mod 7), ..., x == A008578(n) (mod A008578(n+1)) }.
 * https://oeis.org/A116151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer satisfying x ≡ 1 (mod 2), x ≡
 *            2 (mod 3), x ≡ 3 (mod 5), x ≡ 5 (mod 7), …, x ≡ A008578(n) (mod
 *            A008578(n+1)). The implementation applies the Chinese remainder
 *            theorem iteratively over the first n primes
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 347 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

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
    /* Undefined for n == 0; return 0 as a placeholder. */
    if (n == 0)
        return (seq_t)0;

    seq_t x = (seq_t)1;     /* current solution modulo m   */
    seq_t m = (seq_t)1;     /* current modulus (product so far) */

    for (idx_t i = 0; i < n; ++i) {
        /* p = the (i+1)-th prime; residue = previous prime
         *   (or 1 for the first congruence). */
        uint32_t p = get_prime(i + 1);
        seq_t mod     = (seq_t)p;
        seq_t residue = (i == 0) ? (seq_t)1 : (seq_t)get_prime(i);

        /* x ≡ r (mod p)  ⇢  (x + k·m) ≡ r (mod p)  ⇢
         *       k ≡ (r−x)·m⁻¹ (mod p)   (m⁻¹ ≡ m⁻¹ mod p)   */
        seq_t x_mod_p = x % mod;
        seq_t diff;
        if (residue >= x_mod_p) {
            diff = residue - x_mod_p;
        } else {
            /* wrap around: r + mod − x_mod_p */
            diff = residue + mod - x_mod_p;
        }

        seq_t m_mod_p = m % mod;
        seq_t inv = powmod(m_mod_p, (uint64_t)p - 2, mod);  /* inverse of m mod p */

        seq_t delta = (diff * inv) % mod;

        x = x + delta * m;
        m = m * mod;
    }

    return x;
}
