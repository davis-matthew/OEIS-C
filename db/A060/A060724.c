/*
 * A060724  Number of subgroups of the group C_n X C_n (where C_n is the
 *          cyclic group of order n).
 * https://oeis.org/A060724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * tau((n/d)^2)
 * Range:     u8 n <= 35, u16 n <= 2519, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                     (OEIS A060724) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Make sure the cache has at least n+1 slots, initialise new slots to 0 */
static void ensure_cache(idx_t n)
{
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();                 /* out of memory */
        memset(new_cache + cache_size, 0,
               (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }
}

/* Recursively enumerate all divisors of n given its prime factorisation.
   primes  – array of distinct prime factors
   exps    – exponents of those primes in n
   num     – number of distinct primes
   idx     – current depth in recursion
   cur_d   – current divisor value
   cur_tau – current value of tau((n/d)^2) for this divisor
   sum     – pointer to running total (seq_t)                      */
static void dfs(const uint64_t *primes, const int *exps, size_t num,
                size_t idx, seq_t cur_d, seq_t cur_tau, seq_t *sum)
{
    if (idx == num) {
        *sum += cur_d * cur_tau;
        return;
    }

    uint64_t p = primes[idx];
    int e = exps[idx];
    seq_t pow = 1;                                /* p^k */
    for (int k = 0; k <= e; ++k) {
        seq_t new_d   = cur_d * pow;
        seq_t new_tau = cur_tau * ((seq_t)(2 * (e - k) + 1));
        dfs(primes, exps, num, idx + 1, new_d, new_tau, sum);
        pow *= (seq_t)p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, guard */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    /* Special case n = 1 */
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }

    /* Factorise n by simple trial division (n <= 10000 in typical tests) */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[10];
    int exps[10];
    size_t num = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[num] = p;
            exps[num] = e;
            ++num;
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        primes[num] = temp;
        exps[num] = 1;
        ++num;
    }

    seq_t sum = 0;
    dfs(primes, exps, num, 0, (seq_t)1, (seq_t)1, &sum);

    cache[n] = sum;
    return sum;
}
