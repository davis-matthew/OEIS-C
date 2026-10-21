/*
 * A061756  Positive integers k such that k! is divisible by (k+1)^8.
 * https://oeis.org/A061756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, legendre_v
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061756: Positive integers k such that k! is divisible by (k+1)^8. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* build_primes(idx_t limit, idx_t **out, size_t *cnt) */
#include "../utils/legendre_v.h"

static idx_t *primes = NULL;   /* list of primes used for factorisation */
static size_t primes_cnt = 0;
static idx_t primes_max = 0;

/* Computes the integer square root of a 64‑bit value. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Makes sure that @primes contains all primes ≤ @limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_max) return;
    if (primes != NULL) free(primes);
    build_primes(limit, &primes, &primes_cnt);
    primes_max = limit;
}

/* Tests whether k! is divisible by (k+1)^8. */
static bool good_k(seq_t k)
{
    idx_t kk = (idx_t)k;           /* integer value of k */
    idx_t m  = kk + 1;             /* m = k + 1 */
    if (m < 2) return false;       /* never occurs for k≥1 */

    idx_t sqrt_m = isqrt_idx(m);
    ensure_primes(sqrt_m);

    idx_t temp = m;
    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_m; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            idx_t v = legendre_v(kk, p);
            if (v < 8ULL * (idx_t)e)
                return false;
        }
    }
    if (temp > 1) {                  /* remaining prime factor */
        idx_t p = temp;
        idx_t e = 1;
        idx_t v = legendre_v(kk, p);
        if (v < 8ULL * e)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence defined only for n≥1 */

    idx_t count = 0;
    idx_t k = 1;

    while (true) {
        if (good_k((seq_t)k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
