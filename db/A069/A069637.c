/*
 * A069637  Number of prime powers <= n with exponents > 1.
 * https://oeis.org/A069637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥2} π(⌊n^{1/k}⌋) where π(x) is the number of primes ≤
 *            x. This counts every prime power p^e ≤ n with e > 1 exactly once
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 110 terms through n = 9834 (2026-11-01)
 * Helpers:   bit_length_u64_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bit_length_u64_to_u32.h"

/* integer k‑th root: floor(n^(1/k)) */
static inline bool power_leq_uint64(uint64_t base, int exp, uint64_t limit)
{
    __uint128_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
        if (result > limit) return false;
    }
    return true;
}

static inline uint64_t kth_root_uint64(uint64_t n, int k)
{
    unsigned bits = bit_length_u64_to_u32(n);
    unsigned hi_pow = (bits + k - 1) / k;
    uint64_t hi = (hi_pow >= 64) ? n : ((uint64_t)1 << hi_pow);
    if (hi > n) hi = n;
    uint64_t lo = 1;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1); /* upper mid */
        if (power_leq_uint64(mid, k, n))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* prime sieve: build all primes ≤ limit and count them */
static idx_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

static void init_primes(idx_t limit)
{
    if (limit <= prime_limit) return;      /* already covered */

    /* allocate a temporary boolean array */
    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_composite) return;            /* allocation failure – keep old data */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    idx_t *new_arr = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_arr) {
        free(is_composite);
        return;           /* allocation failure – keep old data */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) new_arr[idx++] = i;

    free(is_composite);
    free(primes);

    primes = new_arr;
    prime_cnt = cnt;
    prime_limit = limit;
}

/* binary search for π(x) in the primes array */
static size_t count_primes_leq(idx_t x)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;  /* number of primes ≤ x */
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* smallest prime power with e>1 is 4 */

    /* ensure primes up to sqrt(n) are available */
    idx_t sqrt_n = (idx_t)kth_root_uint64(n, 2);
    init_primes(sqrt_n);

    seq_t total = 0;
    for (int k = 2; ; ++k) {
        uint64_t root = kth_root_uint64(n, k);
        if (root < 2) break;
        total += (seq_t)count_primes_leq((idx_t)root);
    }
    return total;
}
