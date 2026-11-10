/*
 * A083257  a(n) = A071364(n-th nonprime number) = A071364(A018252(n)).
 * https://oeis.org/A083257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A071364(A018252(n)); A018252(n) found by a sieve, then
 *            A071364 = product of the k-th prime to the k-th exponent (sorted
 *            by prime) of its factorization
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 8936 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t A083257_pr[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};

seq_t a(idx_t n)
{
    size_t limit = (size_t)(2 * n + 10);
    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    if (!comp) return 0;
    for (size_t i = 2; i * i <= limit; i++) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }
    /* nonprimes: 1 and composites */
    uint64_t m = 0;
    idx_t count = 0;
    for (size_t k = 1; k <= limit; k++) {
        if (k == 1 || comp[k]) {
            count++;
            if (count == n) { m = (uint64_t)k; break; }
        }
    }
    free(comp);

    if (m <= 1) return (seq_t)1;

    int exps[20];
    int cnt = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            exps[cnt++] = e;
        }
    }
    if (t > 1) exps[cnt++] = 1;

    seq_t result = (seq_t)1;
    for (int i = 0; i < cnt; i++) {
        for (int e = 0; e < exps[i]; e++) result *= (seq_t)A083257_pr[i];
    }
    return result;
}
