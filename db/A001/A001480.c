/*
 * A001480  Let p = A007645(n) be the n-th generalized cuban prime and write p
 *          = x^2 + 3*y^2; a(n) = y.
 * https://oeis.org/A001480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = y where A007645(n) = x^2 + 3*y^2 (x, y >= 0); the primes
 *            A007645 (3 and primes == 1 mod 6) are found with a sieve, and y
 *            by a search over y with 3*y^2 <= p
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001480_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001480_isqrt.h"

static uint64_t *a001480_cp = NULL;
static idx_t a001480_ncp = 0;

static void a001480_build(idx_t n) {
    uint64_t lim = 4096;
    for (;;) {
        unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
        idx_t cnt = 0;
        uint64_t i, j;
        memset(comp, 0, (size_t)(lim + 1));
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 3; i <= lim; i++)
            if (!comp[i] && (i == 3 || i % 6 == 1)) cnt++;
        if (cnt >= n) {
            uint64_t *arr = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
            idx_t k = 0;
            for (i = 3; i <= lim; i++)
                if (!comp[i] && (i == 3 || i % 6 == 1)) arr[k++] = i;
            free(comp);
            free(a001480_cp);
            a001480_cp = arr;
            a001480_ncp = cnt;
            return;
        }
        free(comp);
        lim *= 2;
    }
}

seq_t a(idx_t n) {
    uint64_t p, y;
    if (n < 1) return 0;
    if (n > a001480_ncp) a001480_build(n);
    p = a001480_cp[n - 1];
    for (y = 0; 3 * y * y <= p; y++) {
        uint64_t r = p - 3 * y * y;
        uint64_t x = a001480_isqrt(r);
        if (x * x == r) return (seq_t)y;
    }
    return 0;
}
