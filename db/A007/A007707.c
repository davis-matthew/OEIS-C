/*
 * A007707  Prime(n)*...*prime(a(n)) is the least product of consecutive
 *          primes which is abundant.
 * https://oeis.org/A007707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least k such that prod_{i=n..k} (1 + 1/prime(i))
 *            > 2, found by a segmented sieve of Eratosthenes summing log(1 +
 *            1/p)
 * Range:     u8 n <= 12, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint32_t *bp_list = NULL;
static size_t bp_count = 0;
static uint64_t bp_limit = 0;

static void ensure_base(uint64_t need)
{
    if (bp_limit * bp_limit >= need) return;
    uint64_t L = bp_limit ? bp_limit : 65536;
    while (L * L < need) L *= 2;
    unsigned char *c = (unsigned char *)calloc((size_t)L + 1, 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!c[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= L; j += i) c[j] = 1;
        }
    }
    free(bp_list);
    bp_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    bp_count = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (!c[i]) bp_list[bp_count++] = (uint32_t)i;
    free(c);
    bp_limit = L;
}

seq_t a(idx_t n)
{
    const long double target = logl(2.0L) + 1e-15L;
    const uint64_t SEG = (uint64_t)1 << 18; /* odd numbers per segment */
    long double s = 0.0L;
    uint64_t count = 1; /* the prime 2 */

    if (n == 1) s += log1pl(0.5L);

    unsigned char *mark = (unsigned char *)malloc((size_t)SEG);
    uint64_t lo = 3;
    for (;;) {
        uint64_t hi = lo + 2 * SEG; /* segment covers odd numbers in [lo, hi) */
        ensure_base(hi);
        memset(mark, 0, (size_t)SEG);
        for (size_t k = 1; k < bp_count; k++) {
            uint64_t p = bp_list[k];
            if (p * p >= hi) break;
            uint64_t start = ((lo + p - 1) / p) * p;
            if (start < p * p) start = p * p;
            if ((start & 1) == 0) start += p;
            for (uint64_t j = start; j < hi; j += 2 * p)
                mark[(j - lo) >> 1] = 1;
        }
        for (uint64_t i = 0; i < SEG; i++) {
            if (mark[i]) continue;
            uint64_t p = lo + 2 * i;
            count++;
            if (count >= (uint64_t)n) {
                s += log1pl(1.0L / (long double)p);
                if (s > target) {
                    free(mark);
                    return (seq_t)count;
                }
            }
        }
        lo = hi;
    }
}
