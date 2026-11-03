/*
 * A239963  Number of triangular numbers below prime(n) which are also
 *          primitive roots modulo prime(n).
 * https://oeis.org/A239963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triangular numbers T_k = k(k+1)/2 < prime(n) *
 *            that are primitive roots modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 7109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9814 (2026-11-01)
 * Helpers:   powmod, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/powmod.h"
#include "../utils/smallest_prime_factor.h"

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
    if (n < 1) return 0;

    /* Prime for the given index */
    uint32_t p32 = get_prime(n);
    seq_t p      = (seq_t)p32;
    seq_t p_minus_1 = p - 1;

    /* Distinct prime factors of p-1 */
    uint64_t pf[16];
    size_t pf_cnt = 0;
    uint64_t temp = (uint64_t)p_minus_1;
    while (temp > 1) {
        uint64_t spf = smallest_prime_factor(temp);
        pf[pf_cnt++] = spf;
        while (temp % spf == 0) temp /= spf;
    }

    /* Count triangular numbers that are primitive roots */
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * (k + 1) / 2;
        if (t >= p) break;

        bool is_prim = true;
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t exp = (uint64_t)p_minus_1 / pf[i];
            seq_t r = powmod(t, exp, p);
            if (r == (seq_t)1) {          /* not a primitive root */
                is_prim = false;
                break;
            }
        }
        if (is_prim) count++;
    }

    return count;
}
