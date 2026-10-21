/*
 * A218154  a(n) is the smallest positive integer k such that k^8 + 1 == 0 mod
 *          p, where p is the n-th prime of the form 1 + 16*b (see A094407).
 * https://oeis.org/A218154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest positive integer k such that k^8 + 1 ≡ 0 (mod p_n),
 * where p_n is the n-th prime of the form 1 + 16*b (i.e. p_n ≡ 1 (mod 16)).
 * The function generates the needed prime, finds a primitive root g,
 * then tests the 8 distinct 16th‑roots of unity to locate the minimal k.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num)   */
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base,
                                            uint64_t exp, seq_t mod) */

/* Cache of primes congruent to 1 mod 16.  */
static uint32_t *primes16 = NULL;      /* stored primes ≡1 (mod 16)       */
static size_t primes16_cnt = 0;        /* number stored                   */
static size_t primes16_cap = 0;        /* allocated capacity              */
static uint32_t next_candidate = 17;   /* next odd number ≡1 (mod 16)    */

/* Ensure we have at least n (1‑based) primes ≡1 (mod 16).          */
static void ensure_primes16(idx_t n)
{
    while (primes16_cnt < n) {
        if (is_prime_small((seq_t)next_candidate)) {
            if (primes16_cnt == primes16_cap) {
                size_t new_cap = primes16_cap ? (primes16_cap << 1) : 64;
                void *tmp = realloc(primes16, new_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
                primes16 = (uint32_t*)tmp;
                primes16_cap = new_cap;
            }
            primes16[primes16_cnt++] = next_candidate;
        }
        next_candidate += 16;
    }
}

/* Return the n‑th prime ≡1 (mod 16)  (1‑based).                     */
static uint32_t nth_prime_1mod16(idx_t n)
{
    if (!primes16) { /* initialize on first use */
        primes16 = NULL;
        primes16_cap = 0;
        primes16_cnt = 0;
    }
    ensure_primes16(n);
    return primes16[n - 1];
}

/* Factorize m (m > 1).  Returns the number of distinct prime factors
 * and stores them in pf (must be large enough).                     */
static size_t prime_factors(uint64_t m, uint32_t *pf, size_t max_cnt)
{
    size_t count = 0;
    for (uint64_t i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            if (count < max_cnt) pf[count++] = (uint32_t)i;
            while (m % i == 0) m /= i;
        }
    }
    if (m > 1 && count < max_cnt) pf[count++] = (uint32_t)m;
    return count;
}

/* Finds a primitive root modulo the prime p.  p is assumed > 2.     */
static uint32_t primitive_root(uint32_t p)
{
    uint64_t m = (uint64_t)p - 1;
    uint32_t pf[16];
    size_t pcnt = prime_factors(m, pf, 16);
    for (uint32_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < pcnt && ok; ++i) {
            seq_t val = mod_pow((seq_t)g, m / pf[i], (seq_t)p);
            if (val == 1) ok = false;
        }
        if (ok) return g;
    }
    return 0; /* should never happen for prime p > 2 */
}

/* Main sequence function.                                           */
seq_t a(idx_t n)
{
    uint32_t p = nth_prime_1mod16(n);            /* prime of interest       */
    uint64_t m = (uint64_t)p - 1;                /* group order             */
    uint32_t g = primitive_root(p);              /* primitive root of Z_p*  */

    uint64_t exp_step = m / 8;   /* step between successive 16th roots            */
    uint64_t exp_base = m / 16;  /* base exponent for the first 16th root         */

    seq_t min_k = 0;             /* zero is used as sentinel                        */
    for (uint64_t j = 0; j < 8; ++j) {
        uint64_t exp = exp_base + j * exp_step;
        seq_t k = mod_pow((seq_t)g, exp, (seq_t)p);
        if (k == 0) continue; /* should not happen */
        if (min_k == 0 || k < min_k) min_k = k;
    }
    return min_k;
}
