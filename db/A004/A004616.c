/*
 * A004616  Divisible only by primes congruent to 2 mod 5.
 * https://oeis.org/A004616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve of smallest prime factors; k is a term iff every prime
 *            factor p of k has p == 2 (mod 5), i.e. a(n) = n-th such k (a(1)
 *            = 1)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *A004616_terms = NULL;
static size_t A004616_count = 0;

static void A004616_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    unsigned char *good = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint64_t i, j;
    size_t cnt = 0;

    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    good[1] = 1;
    cnt = 1;
    for (i = 2; i <= limit; i++) {
        uint32_t p = spf[i];
        if (p % 5 == 2 && good[i / p]) {
            good[i] = 1;
            cnt++;
        }
    }

    free(A004616_terms);
    A004616_terms = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    A004616_count = 0;
    for (i = 1; i <= limit; i++)
        if (good[i]) A004616_terms[A004616_count++] = i;

    free(spf);
    free(good);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;

    if (n < 1) return (seq_t)0;
    if (A004616_terms == NULL || (uint64_t)A004616_count < n) {
        if (limit == 0) limit = 1024;
        for (;;) {
            A004616_build(limit);
            if ((uint64_t)A004616_count >= n) break;
            limit *= 2;
        }
    }
    return (seq_t)A004616_terms[n - 1];
}
