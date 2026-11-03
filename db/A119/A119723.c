/*
 * A119723  Numerator of Sum[ (-1)^(k-1) * 1/(Prime[k]-1)^2, {k,1,n}].
 * https://oeis.org/A119723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Sum[ (-1)^(k-1) * 1/(Prime[k]-1)^2, {k,1,n}]
 *            evaluated in lowest terms. The function returns a(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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
    seq_t num = 0;      /* numerator of the running sum   */
    seq_t den = 1;      /* denominator of the running sum */

    for (idx_t k = 1; k <= n; ++k) {
        /* kth prime (1‑based) */
        uint32_t p = get_prime(k);                 /* prime[k]   */
        seq_t d = (seq_t)(p - 1);                  /* d = prime-1 */
        d = d * d;                                 /* d = (prime-1)^2 */

        /* sign for this term: + for odd k, - for even k */
        int sign = (k & 1U) ? 1 : -1;

        /* Compute gcd and lcm parts to avoid overflow */
        seq_t g = gcd(den, d);          /* gcd(den, d)   */
        seq_t mult = d / g;             /* factor for current numerator */
        seq_t term = den / g;           /* factor for the added term   */

        /* Update numerator: new_num = num * mult ± term */
        seq_t new_num = num * mult;
        if (sign > 0)
            new_num += term;
        else
            new_num -= term;           /* guaranteed non‑negative */

        /* Update denominator: new_den = (den / g) * d */
        seq_t new_den = term * d;       /* term = den / g */

        /* Reduce fraction to lowest terms again */
        seq_t g2 = gcd(new_num, new_den);
        if (g2 != 1) {
            new_num /= g2;
            new_den /= g2;
        }

        /* Prepare for next iteration */
        num = new_num;
        den = new_den;
    }

    return num;
}
