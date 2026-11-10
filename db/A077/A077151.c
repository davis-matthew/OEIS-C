/*
 * A077151  Index of the first occurrence of n in A077150, or 0 if n does not
 *          occur.
 * https://oeis.org/A077151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>=1 with A077150(k) = n, else 0, where A077150(k)
 *            = phi(k) + pi(k) - pi(2k); found by a sieve and search up to k =
 *            20n+2000
 * Range:     u8 n <= 48, u16 n <= 9074, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9911 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint32_t *A077151_first = NULL;
static uint64_t A077151_valid = 0;   /* table correct for all m <= this */
static bool A077151_built = false;

static void A077151_build(uint64_t m)
{
    uint64_t L = 20 * m + 2000;
    uint64_t N = 2 * L;
    uint32_t *phi = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    uint32_t *pi = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)malloc(N + 1);
    uint32_t *first = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    if (!phi || !pi || !comp || !first) exit(1);
    for (uint64_t i = 0; i <= N; i++) comp[i] = 0;
    for (uint64_t i = 0; i <= L; i++) { phi[i] = (uint32_t)i; first[i] = 0; }
    for (uint64_t p = 2; p <= N; p++) {
        if (!comp[p]) {
            if (p <= L)
                for (uint64_t j = p; j <= L; j += p) phi[j] -= phi[j] / (uint32_t)p;
            if (p * p <= N)
                for (uint64_t j = p * p; j <= N; j += p) comp[j] = 1;
        }
    }
    pi[0] = 0;
    pi[1] = 0;
    for (uint64_t i = 2; i <= N; i++) pi[i] = pi[i - 1] + (comp[i] ? 0 : 1);
    for (uint64_t k = 1; k <= L; k++) {
        long long v = (long long)phi[k] + (long long)pi[k] - (long long)pi[2 * k];
        if (v >= 0 && (uint64_t)v <= L && first[v] == 0)
            first[v] = (uint32_t)k;
    }
    free(phi); free(pi); free(comp);
    if (A077151_built) free(A077151_first);
    A077151_first = first;
    A077151_valid = m;
    A077151_built = true;
}

seq_t a(idx_t n)
{
    if (!A077151_built || (uint64_t)n > A077151_valid) {
        uint64_t m = (uint64_t)n;
        if (m < 1000) m = 1000;
        if (A077151_built && m < 2 * A077151_valid) m = 2 * A077151_valid;
        A077151_build(m);
    }
    return (seq_t)A077151_first[(uint64_t)n];
}
