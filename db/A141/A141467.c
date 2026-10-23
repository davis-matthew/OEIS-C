/*
 * A141467  a(1)=1; for n > 1, a(n) is the product of prime factors of the
 *          n-th composite, but with the largest prime incremented by 3 and
 *          the smallest decremented by 1.
 * https://oeis.org/A141467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k*(pmax+3)*(pmin-1)/(pmin*pmax) for n > 1, a(1) = 1,
 *            where k is the n-th composite, pmin the smallest prime factor of
 *            k, and pmax the largest prime factor of k. If the result is
 *            prime or < 2, the sequence value is 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Dynamically grow a buffer of arbitrary element size. */
static void *grow_array(void *current, size_t *cap, size_t elem_size, size_t needed) {
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;
        void *ptr = realloc(current, newcap * elem_size);
        if (!ptr) exit(EXIT_FAILURE);
        *cap = newcap;
        return ptr;
    }
    return current;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n = 0 */

    /* Special value for the first element of the sequence. */
    if (n == 1) return (seq_t)1;

    /* Static cache state, preserved across calls. */
    static uint32_t *primes      = NULL;
    static size_t   prime_cnt    = 0, prime_cap = 0;
    static uint64_t *comps      = NULL;
    static seq_t    *vals       = NULL;
    static size_t   comp_cnt    = 0, comp_cap = 0, val_cap = 0;
    static uint64_t last_checked = 1;   /* number previously examined */

    /* Grow buffers to hold the requested length. */
    while (comp_cnt < (size_t)n) {
        ++last_checked;                      /* next integer to test */

        /* Check if the integer is prime. */
        if (is_prime_small((seq_t)last_checked)) {
            primes = (uint32_t *)grow_array(primes, &prime_cap, sizeof(uint32_t), prime_cnt + 1);
            primes[prime_cnt++] = (uint32_t)last_checked;
            continue;
        }

        /* Composite number: factor it using the primes encountered so far. */
        uint64_t m     = last_checked;
        uint64_t temp  = m;
        uint64_t pmin  = 0;
        uint64_t pmax  = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = (uint64_t)primes[i];
            unsigned __int128 sq = (unsigned __int128)p * p;
            if (sq > temp) break;
            if (temp % p == 0) {
                if (pmin == 0) pmin = p;
                pmax = p;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) { /* Remaining factor is prime. */
            if (pmin == 0) pmin = temp;
            pmax = temp;
        }

        /* Compute the transformed product. */
        seq_t prod = (seq_t)m;
        seq_t denom = (seq_t)pmin * (seq_t)pmax;
        prod = prod / denom;
        prod = prod * (seq_t)(pmin - 1);
        prod = prod * (seq_t)(pmax + 3);

        /* If the result is prime or < 2, set to 1. */
        if (prod < (seq_t)2 || is_prime_big(prod))
            prod = (seq_t)1;

        /* Cache the result. */
        comps = (uint64_t *)grow_array(comps, &comp_cap, sizeof(uint64_t), comp_cnt + 1);
        comps[comp_cnt] = m;

        vals = (seq_t *)grow_array(vals, &val_cap, sizeof(seq_t), comp_cnt + 1);
        vals[comp_cnt] = prod;

        ++comp_cnt;
    }

    /* For n > 1, the corresponding value is stored at index n-1. */
    return vals[n - 1];
}
