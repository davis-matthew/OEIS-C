/*
 * A116569  a(n) = (x^3 - x) / 6, where x is the genus of the modular curve
 *          X_0(p) for p = prime(n).
 * https://oeis.org/A116569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (x^3 - x) / 6, where x is the genus of the modular curve
 *            X0(p_n) * for the n-th prime p_n. The genus for a prime p > 3 is
 *            * * g = floor((p + 1) / 12) , * with the adjustment g -= 1 if p
 *            > 13 and p ≡ 1 (mod 12)
 * Range:     u8 n <= 34, u16 n <= 153, u32 n <= 2228, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9344 (2026-11-01)
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
    /* Retrieve the n-th prime (1‑based index). */
    uint64_t p = get_prime(n);

    /* Compute the genus of X0(p). */
    uint64_t g_val = (p + 1) / 12;
    if (p > 13u && (p % 12u) == 1u)
        g_val -= 1u;
    seq_t g = (seq_t)g_val;

    /* Compute a(n) = (g^3 - g) / 6. */
    seq_t res = (g * g * g - g) / 6;
    return res;
}
