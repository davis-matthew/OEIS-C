/*
 * A084111  Numbers m such that A084110(m) = m.
 * https://oeis.org/A084111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with A084110(m) = m, where A084110(m) folds the
 *            sorted divisors d_i of m: e_1 = 1, e_i = e_{i-1}/d_i if d_i
 *            divides e_{i-1}, else e_{i-1}*d_i; A084110(m) = e_k. Computed by
 *            a search over m; e is tracked as a prime-exponent vector, so
 *            there is no overflow
 * Range:     u8 n <= 60, u16 n <= 6774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9966 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static int A084111_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static int A084111_is_fixed(uint64_t m)
{
    uint64_t pr[20];
    int ex[20];
    int cnt = 0;
    uint64_t n = m;
    uint64_t p;
    if (n % 2 == 0) {
        int e = 0;
        while (n % 2 == 0) { n /= 2; e++; }
        pr[cnt] = 2; ex[cnt] = e; cnt++;
    }
    for (p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; e++; }
            pr[cnt] = p; ex[cnt] = e; cnt++;
        }
    }
    if (n > 1) { pr[cnt] = n; ex[cnt] = 1; cnt++; }

    if (cnt == 1 && ex[0] == 1) return 1;   /* prime */

    size_t total = 1;
    for (int i = 0; i < cnt; i++) total *= (size_t)(ex[i] + 1);
    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    size_t dc = 1;
    divs[0] = 1;
    for (int i = 0; i < cnt; i++) {
        size_t cur = dc;
        uint64_t mult = 1;
        for (int k = 1; k <= ex[i]; k++) {
            mult *= pr[i];
            for (size_t j = 0; j < cur; j++) divs[dc++] = divs[j] * mult;
        }
    }
    qsort(divs, dc, sizeof(uint64_t), A084111_cmp);

    long long ev[20];
    for (int i = 0; i < cnt; i++) ev[i] = 0;
    for (size_t t = 0; t < dc; t++) {
        uint64_t d = divs[t];
        long long dv[20];
        int divides = 1;
        for (int i = 0; i < cnt; i++) {
            int e = 0;
            while (d % pr[i] == 0) { d /= pr[i]; e++; }
            dv[i] = e;
            if (e > ev[i]) divides = 0;
        }
        for (int i = 0; i < cnt; i++) {
            if (divides) ev[i] -= dv[i]; else ev[i] += dv[i];
        }
    }
    free(divs);
    for (int i = 0; i < cnt; i++) if (ev[i] != ex[i]) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t count = 0, cap = 0;
    static uint64_t next_m = 1;
    while (count < (size_t)n) {
        if (A084111_is_fixed(next_m)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[count++] = next_m;
        }
        next_m++;
    }
    return (seq_t)terms[(size_t)n - 1];
}
