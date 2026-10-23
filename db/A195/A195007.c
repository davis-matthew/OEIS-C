/*
 * A195007  Number of primes in the range (n*sqrt(n-1), (n+1)*sqrt(n)].
 * https://oeis.org/A195007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p such that n*sqrt(n-1) < p <=
 *            (n+1)*sqrt(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


static uint64_t floor_sqrt_u64(uint64_t x) {
    uint64_t low = 0, high = x, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

static uint32_t *prime_count = NULL;
static uint64_t sieve_limit = 0;

static void ensure_sieve_limit(uint64_t limit) {
    if (sieve_limit >= limit) return;
    uint64_t n = limit;
    uint32_t *new_pc = (uint32_t*)malloc((n + 1) * sizeof(uint32_t));
    if (!new_pc) exit(1);
    bool *is_comp = (bool*)calloc(n + 1, 1);
    if (!is_comp) { free(new_pc); exit(1); }
    for (uint64_t i = 2; i * i <= n; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= n; j += i) is_comp[j] = true;
        }
    }
    uint32_t cnt = 0;
    new_pc[0] = new_pc[1] = 0;
    for (uint64_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) ++cnt;
        new_pc[i] = cnt;
    }
    free(is_comp);
    free(prime_count);
    prime_count = new_pc;
    sieve_limit = n;
}

seq_t a(idx_t n) {
    uint64_t n64 = (uint64_t)n;
    uint64_t n1 = n64 - 1;
    unsigned __int128 A128 = (unsigned __int128)n64 * n64 * n1;
    unsigned __int128 B128 = (unsigned __int128)(n64 + 1) * (n64 + 1) * n64;
    uint64_t A = (uint64_t)A128;
    uint64_t B = (uint64_t)B128;
    uint64_t lower = floor_sqrt_u64(A);
    uint64_t upper = floor_sqrt_u64(B);
    ensure_sieve_limit(upper);
    uint32_t count = prime_count[upper] - prime_count[lower];
    return (seq_t)count;
}
