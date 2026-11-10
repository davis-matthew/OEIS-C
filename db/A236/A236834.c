/*
 * A236834  Numbers that do not occur as results of "downward"
 *          remultiplication (N -> GF(2)[X]) of any number; numbers not
 *          present in A234741.
 * https://oeis.org/A236834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: a(n) = n-th positive integer that is not a carryless
 *            (GF(2)[X]) product of ordinary primes, i.e. not a value of
 *            A234741
 * Range:     u8 n <= 27, u16 n <= 2525, u32 n <= 2525, u64 n <= 2525,
 *            u128 n <= 2525, big any n
 * Validated: standard, pass, 71 terms through n = 2454 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A236834_list = NULL;
static uint64_t A236834_cnt = 0;

static unsigned A236834_bl(uint64_t x)
{
    unsigned b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static uint64_t A236834_clmul(uint64_t x, uint64_t y)
{
    uint64_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

/* Mark every carryless product of primes below 2^B, collect the unmarked numbers */
static void A236834_build(unsigned B)
{
    uint64_t N = (uint64_t)1 << B, i, j;
    unsigned char *comp = (unsigned char *)calloc(N + 1, 1);
    unsigned char *reach = (unsigned char *)calloc(N + 1, 1);
    uint32_t *primes = (uint32_t *)malloc(sizeof(uint32_t) * (N / 2 + 2));
    uint64_t np = 0;
    for (i = 2; i < N; i++) {
        if (!comp[i]) {
            primes[np++] = (uint32_t)i;
            for (j = i * i; j < N; j += i) comp[j] = 1;
        }
    }
    reach[1] = 1;
    for (i = 1; i < N; i++) {
        if (!reach[i]) continue;
        unsigned di = A236834_bl(i);
        for (j = 0; j < np; j++) {
            unsigned dp = A236834_bl(primes[j]);
            /* product bit length = di + dp - 1; must be <= B to be < 2^B */
            if (di + dp - 1 > B) break;
            reach[A236834_clmul(i, primes[j])] = 1;
        }
    }
    free(A236834_list);
    A236834_list = (uint32_t *)malloc(sizeof(uint32_t) * (N + 1));
    A236834_cnt = 0;
    for (i = 1; i < N; i++)
        if (!reach[i]) A236834_list[A236834_cnt++] = (uint32_t)i;
    free(comp);
    free(reach);
    free(primes);
}

seq_t a(idx_t n)
{
    static unsigned B = 0;
    if (n == 0) return 0;
    if (B == 0) { B = 8; A236834_build(B); }
    while (A236834_cnt < (uint64_t)n) {
        B++;
        A236834_build(B);
    }
    return (seq_t)A236834_list[(uint64_t)n - 1];
}
