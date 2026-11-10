/*
 * A098047  Numbers not in A098006.
 * https://oeis.org/A098047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive k with no odd prime p such that (p-1)/2 -
 *            phi(p-1) = k. Method: write p-1 = 2^a*r (r odd); then value =
 *            2^(a-1)*(r-phi(r)). Sieve phi(r) for odd r <= K^2, mark every
 *            reachable value <= K when 2^a*r+1 is prime, and list the
 *            unmarked values; double K as needed
 * Range:     u8 n <= 100, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 953 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A098047_terms = NULL;
static uint64_t A098047_cnt = 0;

static void A098047_build(uint64_t K)
{
    uint64_t R = K * K;
    uint32_t *phi = (uint32_t *)malloc((R + 1) * sizeof(uint32_t));
    uint8_t *mark = (uint8_t *)calloc(K + 1, 1);
    if (!phi || !mark) abort();
    for (uint64_t i = 0; i <= R; i++) phi[i] = (uint32_t)i;
    for (uint64_t p = 2; p <= R; p++) {
        if (phi[p] == (uint32_t)p) {
            for (uint64_t j = p; j <= R; j += p)
                phi[j] -= phi[j] / (uint32_t)p;
        }
    }
    for (uint64_t r = 3; r <= R; r += 2) {
        uint64_t s = r - phi[r];
        if (s > K) continue;
        for (int a = 1; (s << (a - 1)) <= K; a++) {
            uint64_t k = s << (a - 1);
            if (!mark[k] && is_prime_u64(((uint64_t)r << a) + 1ULL))
                mark[k] = 1;
        }
    }
    free(A098047_terms);
    A098047_terms = (uint64_t *)malloc((K + 1) * sizeof(uint64_t));
    if (!A098047_terms) abort();
    A098047_cnt = 0;
    for (uint64_t k = 1; k <= K; k++)
        if (!mark[k]) A098047_terms[A098047_cnt++] = k;
    free(phi);
    free(mark);
}

seq_t a(idx_t n)
{
    static uint64_t K = 0;
    if (n == 0) return (seq_t)0;
    while (A098047_cnt < (uint64_t)n) {
        K = K ? K * 2 : 256;
        A098047_build(K);
    }
    return (seq_t)A098047_terms[n - 1];
}
