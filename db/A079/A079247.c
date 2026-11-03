/*
 * A079247  Number of pairs (p,q), 0 <= p < q, such that p+q divides n.
 * https://oeis.org/A079247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9245 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A079247: Number of pairs (p,q), 0 ≤ p < q, such that p+q divides n.
    Formula: a(n) = Σ_{d|n} floor((d+1)/2)                      (n ≥ 1)   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Recursive enumeration of all divisors of n.
   cur      – product of chosen prime powers so far (a divisor)
   pr, exp  – prime factorisation of n
   cnt      – number of distinct primes
   sum      – pointer to accumulator of floor((divisor+1)/2)   */
static void enumerate_divs(size_t idx, seq_t cur,
                           const uint64_t *pr, const int *exp,
                           size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        *sum += (cur + (seq_t)1) / (seq_t)2;   /* add floor((div+1)/2) */
        return;
    }
    seq_t pow = (seq_t)1;
    for (int e = 0; e <= exp[idx]; ++e) {
        enumerate_divs(idx + 1, cur * pow, pr, exp, cnt, sum);
        pow *= (seq_t)pr[idx];
    }
}

/* Memoised computation of a(n).  Cache is indexed by n (≥1). */
seq_t a(idx_t n)
{
    /* Static cache storing previously computed values.
       cache[0] is unused; indices 1..cache_cap are valid. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* greatest index stored */

    /* Return cached value if available. */
    if (cache && n <= cache_cap && cache[n] != 0) {
        return cache[n];
    }

    /* Ensure cache large enough.  Allocate n+1 elements to use index n. */
    if (cache == NULL) {
        idx_t new_cap = n;
        cache = mem_new(seq_t, (new_cap + 1));
        for (idx_t i = 0; i <= new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    } else if (n > cache_cap) {
        idx_t old_cap = cache_cap;
        idx_t new_cap = n;
        seq_t *new_cache = mem_resize(seq_t, cache, (new_cap + 1));
        if (new_cache == NULL) {           /* allocation failed – abort */
            abort();
        }
        for (idx_t i = old_cap + 1; i <= new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Factorise n.  Since n ≤ 10000 in the intended test range,
       trial division up to √n is fast enough. */
    uint64_t m = (uint64_t)n;
    uint64_t primes[32];
    int       exps[32];
    size_t    cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            do {
                m /= p;
                ++e;
            } while (m % p == 0);
            primes[cnt] = p;
            exps[cnt++] = e;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Enumerate all divisors and accumulate the sum. */
    seq_t sum = (seq_t)0;
    enumerate_divs(0, (seq_t)1, primes, exps, cnt, &sum);

    /* Store and return.   The sum can never be zero for n ≥ 1. */
    cache[n] = sum;
    return sum;
}
