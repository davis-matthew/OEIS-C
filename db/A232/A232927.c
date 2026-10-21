/*
 * A232927  a(n) is the smallest k such that the first k primes generate the
 *          multiplicative group modulo n.
 * https://oeis.org/A232927
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the first k primes generate the *
 *            multiplicative group modulo n (OEIS A232927)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_int.h"

/* local prime table – enough primes up to 20000 for n ≤ 10000 */
static uint32_t primes[2262];
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (prime_cnt) return;
    const idx_t LIMIT = 20000;
    bool *is_comp = (bool*)calloc(LIMIT + 1, 1);
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) primes[prime_cnt++] = (uint32_t)i;
    }
    free(is_comp);
}

/* simple gcd for 32‑bit values */

/* Euler phi for small n using the prime table */
static idx_t phi_n(idx_t n) {
    idx_t result = n;
    idx_t temp = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

static unsigned char visited[10001];
static idx_t group_elems[10001];

seq_t a(idx_t n) {
    if (n < 3) return 0;          /* undefined for n < 3, but never used */
    init_primes();

    idx_t phi = phi_n(n);
    memset(visited, 0, (size_t)n);
    size_t group_len = 1;
    group_elems[0] = 1;
    visited[1] = 1;

    size_t k = 0;
    for (size_t pi = 0; group_len < phi && pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        ++k;                         /* count this prime in k regardless of coprimality */
        if (gcd_int((int)p, (int)n) != 1) continue; /* not coprime, no effect on group */

        /* Expand the existing subgroup with this new prime p */
        size_t i = 0;
        while (i < group_len) {
            idx_t r = group_elems[i];
            idx_t t = (r * (idx_t)p) % n;
            if (!visited[t]) {
                visited[t] = 1;
                group_elems[group_len++] = t;
            }
            ++i;
        }
    }
    return (seq_t)k;
}
