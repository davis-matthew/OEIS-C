/*
 * A098901  Duplicate of A098767.
 * https://oeis.org/A098901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th number k such that k and its decimal reversal are
 *            both deficient (sigma(k) < 2k); computed by search with
 *            trial-division sigma
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned long long *A098901_cache = NULL;
static size_t A098901_len = 0, A098901_cap = 0;
static unsigned long long A098901_next = 1;

static bool A098901_deficient(unsigned long long x) {
    unsigned long long sum = 1, m = x;
    if (x == 1) return true;
    for (unsigned long long p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned long long part = 1, pw = 1;
            while (m % p == 0) { m /= p; pw *= p; part += pw; }
            sum *= part;
        }
    }
    if (m > 1) sum *= (1 + m);
    return sum < 2 * x;
}

static unsigned long long A098901_rev(unsigned long long x) {
    unsigned long long r = 0;
    while (x > 0) { r = r * 10 + x % 10; x /= 10; }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A098901_len < (size_t)n) {
        unsigned long long c = A098901_next++;
        if (A098901_deficient(c) && A098901_deficient(A098901_rev(c))) {
            if (A098901_len == A098901_cap) {
                size_t nc = A098901_cap ? A098901_cap * 2 : 64;
                unsigned long long *t = (unsigned long long *)realloc(A098901_cache, nc * sizeof(unsigned long long));
                if (!t) exit(1);
                A098901_cache = t;
                A098901_cap = nc;
            }
            A098901_cache[A098901_len++] = c;
        }
    }
    return (seq_t)A098901_cache[n - 1];
}
