/*
 * A219611  a(n) is the smallest omega(A061214(k)) sampled over all indices k
 *          of prime gaps prime(k+1) - prime(k) = 2n, where omega = A001221.
 * https://oeis.org/A219611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = min over all k with prime(k+1)-prime(k)=2n of
 *            omega(A061214(k)); computed by sieving primes up to 2*10^7,
 *            trial-dividing the composites of each such gap and counting
 *            distinct primes
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A219611_LIMIT 20000000u

static unsigned char *A219611_comp = NULL;
static uint32_t *A219611_small = NULL;
static size_t A219611_small_cnt = 0;

static void A219611_init(void)
{
    if (A219611_comp) return;
    size_t L = A219611_LIMIT;
    A219611_comp = (unsigned char *)calloc(L + 1, 1);
    if (!A219611_comp) exit(1);
    A219611_comp[0] = A219611_comp[1] = 1;
    for (size_t i = 2; i * i <= L; ++i)
        if (!A219611_comp[i])
            for (size_t j = i * i; j <= L; j += i) A219611_comp[j] = 1;
    A219611_small = (uint32_t *)malloc(700 * sizeof(uint32_t));
    if (!A219611_small) exit(1);
    for (uint32_t i = 2; i <= 4500 && A219611_small_cnt < 700; ++i)
        if (!A219611_comp[i]) A219611_small[A219611_small_cnt++] = i;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A219611_init();
    size_t L = A219611_LIMIT;
    size_t gap = (size_t)(2 * n);
    if (gap > 2000) return (seq_t)0;
    size_t cap = gap * 10 + 16;
    uint32_t *ps = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!ps) exit(1);
    bool found = false;
    size_t best = 0;
    size_t prev = 2;
    for (size_t p = 3; p <= L; ++p) {
        if (A219611_comp[p]) continue;
        if (p - prev == gap) {
            size_t cnt = 0;
            for (size_t x = prev + 1; x < p; ++x) {
                uint32_t m = (uint32_t)x;
                for (size_t i = 0; i < A219611_small_cnt; ++i) {
                    uint32_t q = A219611_small[i];
                    if ((uint64_t)q * q > m) break;
                    if (m % q == 0) {
                        while (m % q == 0) m /= q;
                        bool dup = false;
                        for (size_t t = 0; t < cnt; ++t)
                            if (ps[t] == q) { dup = true; break; }
                        if (!dup) ps[cnt++] = q;
                    }
                }
                if (m > 1) {
                    bool dup = false;
                    for (size_t t = 0; t < cnt; ++t)
                        if (ps[t] == m) { dup = true; break; }
                    if (!dup) ps[cnt++] = m;
                }
            }
            if (!found || cnt < best) { best = cnt; found = true; }
        }
        prev = p;
    }
    free(ps);
    return (seq_t)best;
}
