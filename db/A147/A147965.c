/*
 * A147965  a(n) = n + 1 - A001223(n) = n - A046933(n). In words, a(n) is the
 *          difference between n+1 and the n-th gap between primes.
 * https://oeis.org/A147965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = n+1 - (p_{n+1} - p_n) (gap between consecutive primes)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square‑root: returns floor(sqrt(n)) for n <= UINT64_MAX. */
static inline idx_t isqrt_u64(uint64_t x)
{
    idx_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* static cache of primes, 0‑based: prime[0] = 2 */
    static uint64_t *prime = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;

    /* need primes 0 … n  (i.e. n+1 primes) */
    size_t target = (size_t)n + 1;
    if (prime_cnt < target) {
        /* grow the array if needed */
        if (prime_cap < target) {
            size_t new_cap = prime_cap ? prime_cap << 1 : 16;
            while (new_cap < target) new_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(prime, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime = tmp;
            prime_cap = new_cap;
        }

        /* initialize first prime if necessary */
        if (prime_cnt == 0) {
            prime[0] = 2;
            prime_cnt = 1;
        }

        /* find next primes until we have enough */
        uint64_t cand = prime[prime_cnt - 1] + 1;
        if ((cand & 1ULL) == 0)  /* start with an odd candidate */
            cand++;
        while (prime_cnt < target) {
            bool ok = true;
            idx_t lim = isqrt_u64(cand);
            for (size_t i = 0; i < prime_cnt && prime[i] <= (uint64_t)lim; ++i) {
                if (cand % prime[i] == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                prime[prime_cnt++] = cand;
            }
            cand += 2;  /* skip even numbers */
        }
    }

    /* gap = prime_{n+1} - prime_n  (indices 0‑based) */
    uint64_t gap = prime[(size_t)n] - prime[(size_t)n - 1];

    /* a(n) = n + 1 - gap */
    idx_t result = (idx_t)n + 1 - (idx_t)gap;
    return (seq_t)result;
}
