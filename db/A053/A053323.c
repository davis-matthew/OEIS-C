/*
 * A053323  First differences of A031928.
 * https://oeis.org/A053323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A031928(n+1) - A031928(n), where A031928 lists the lower
 *            primes p with nextprime(p) - p = 10; computed by a sieve that is
 *            doubled until n+1 gaps are found
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9146 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *A053323_lows = NULL;
static size_t A053323_cnt = 0;
static uint64_t A053323_limit = 0;

static void A053323_build(uint64_t limit)
{
    unsigned char *s = (unsigned char *)malloc((size_t)limit + 1);
    if (!s) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= limit; ++i) s[i] = 1;
    s[0] = 0;
    if (limit >= 1) s[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (s[i])
            for (uint64_t j = i * i; j <= limit; j += i) s[j] = 0;
    size_t cap = 1024, c = 0;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!arr) exit(EXIT_FAILURE);
    uint64_t prev = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (s[i]) {
            if (prev && i - prev == 10) {
                if (c == cap) {
                    cap *= 2;
                    arr = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
                    if (!arr) exit(EXIT_FAILURE);
                }
                arr[c++] = prev;
            }
            prev = i;
        }
    }
    free(s);
    free(A053323_lows);
    A053323_lows = arr;
    A053323_cnt = c;
    A053323_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t need = (size_t)n + 1;
    if (A053323_cnt < need) {
        uint64_t lim = A053323_limit ? A053323_limit * 2 : 100000;
        for (;;) {
            A053323_build(lim);
            if (A053323_cnt >= need) break;
            lim *= 2;
        }
    }
    return (seq_t)(A053323_lows[n] - A053323_lows[n - 1]);
}
