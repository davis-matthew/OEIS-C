/*
 * A226734  a(n) = A002110(n) mod A000217(n).
 * https://oeis.org/A226734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002110(n) mod A000217(n) (product of the first n primes)
 *            mod (n*(n+1)/2)
 * Range:     u8 n <= 22, u16 n <= 403, u32 n <= 1383, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t* primes = NULL;      /* array of found primes   */
static size_t   primes_cap = 0;      /* allocated capacity     */
static size_t   primes_cnt = 0;      /* number of primes stored */

/* extend the primes array to contain at least 'need' primes   */
static void ensure_primes(size_t need)
{
    /* growth strategy: double capacity each time */
    if (primes_cnt >= need) return;
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }
    while (primes_cnt < need) {
        /* generate next prime */
        uint64_t cand = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
        /* increase candidate until prime */
        while (1) {
            int is_prime = 1;
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint64_t p = primes[j];
                if (p > cand / p) break;                 /* p*p > cand */
                if (cand % p == 0) { is_prime = 0; break; }
            }
            if (is_prime) break;
            ++cand;
        }
        /* store the prime */
        if (primes_cnt == primes_cap) {
            primes_cap <<= 1;
            primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_cnt++] = cand;
    }
}

seq_t a(idx_t n)
{
    /* we assume n >= 1 according to OEIS offset */
    ensure_primes((size_t)n);          /* ensure we have at least n primes */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number */

    seq_t prod = 1;
    for (size_t i = 0; i < (size_t)n; ++i)
        prod = (prod * (seq_t)primes[i]) % tri;

    return prod;
}
