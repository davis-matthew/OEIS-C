/*
 * A240718  Number of decompositions of 2n into an unordered sum of two
 *          primes, one of the two primes less than sqrt(2n-2).
 * https://oeis.org/A240718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered prime pairs (p,q) with p+q = 2n, p <
 *            sqrt(2n‑2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        /* use 128‑bit to avoid overflow for mid*mid */
        if ((unsigned __int128)mid * mid <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Generate all primes up to `limit` (inclusive) and store them in
   `*primes_out`. The caller owns the memory and must free it. */
static void generate_primes_upto(uint64_t limit, uint64_t **primes_out, size_t *cnt_out) {
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    size_t cap = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cap;
    uint64_t *arr = (uint64_t*)malloc(cap * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) arr[idx++] = i;
    free(is_prime);
    *primes_out = arr;
    *cnt_out = cap;
}

/* Simple primality test using trial division by primes up to sqrt(n). */
static bool is_prime_uint64(uint64_t n, uint64_t *primes, size_t primes_cnt) {
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    idx_t sum =  (idx_t)2 * n;
    /* Compute the floor of sqrt(2*n-2). */
    uint64_t limit = isqrt_u64((uint64_t)sum - 2);
    /* Generate all primes up to 'limit'. */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    generate_primes_upto(limit, &primes, &primes_cnt);

    seq_t cnt = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((uint64_t)p > limit) break;
        uint64_t q = (uint64_t)sum - p;
        if (q < p) continue;          /* unordered pairs counted once */
        if (is_prime_uint64(q, primes, primes_cnt)) ++cnt;
    }
    free(primes);
    return cnt;
}
