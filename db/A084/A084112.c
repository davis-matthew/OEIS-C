/*
 * A084112  Nonprimes in A084111.
 * https://oeis.org/A084112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite-or-1 m with A084110(m) = m (i.e. m in
 *            A084111 and A010051(m)=0); computed by a memoized search over m,
 *            tracking the A084110 iteration as a prime-exponent vector (no
 *            overflow)
 * Range:     u8 n <= 6, u16 n <= 232, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 968 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static int A084112_cmp(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p < q) ? -1 : (p > q) ? 1 : 0;
}

/* returns 1 if m is a fixed point of A084110 and not prime */
static int A084112_check(uint64_t m)
{
    uint64_t pr[24];
    int pe[24];
    int pc = 0;
    uint64_t t = m;
    uint64_t p;
    if (m == 1) return 1;
    if ((t & 1ULL) == 0) {
        int e = 0;
        while ((t & 1ULL) == 0) { t >>= 1; e++; }
        pr[pc] = 2; pe[pc] = e; pc++;
    }
    for (p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            pr[pc] = p; pe[pc] = e; pc++;
        }
    }
    if (t > 1) { pr[pc] = t; pe[pc] = 1; pc++; }
    if (pc == 1 && pe[0] == 1) return 0; /* prime */

    size_t cap = 1;
    int j;
    for (j = 0; j < pc; j++) cap *= (size_t)(pe[j] + 1);
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) return 0;
    size_t cnt = 1;
    divs[0] = 1;
    for (j = 0; j < pc; j++) {
        size_t base = cnt;
        uint64_t mult = 1;
        int k;
        for (k = 1; k <= pe[j]; k++) {
            mult *= pr[j];
            size_t i;
            for (i = 0; i < base; i++) divs[cnt++] = divs[i] * mult;
        }
    }
    qsort(divs, cnt, sizeof(uint64_t), A084112_cmp);

    int ex[24];
    for (j = 0; j < pc; j++) ex[j] = 0;
    size_t i;
    for (i = 0; i < cnt; i++) {
        uint64_t d = divs[i];
        int de[24];
        int ok = 1;
        for (j = 0; j < pc; j++) {
            int e = 0;
            while (d % pr[j] == 0) { d /= pr[j]; e++; }
            de[j] = e;
            if (ex[j] < e) ok = 0;
        }
        if (ok) {
            for (j = 0; j < pc; j++) ex[j] -= de[j];
        } else {
            for (j = 0; j < pc; j++) ex[j] += de[j];
        }
    }
    free(divs);
    for (j = 0; j < pc; j++)
        if (ex[j] != pe[j]) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t nt = 0, cap = 0;
    static uint64_t next_m = 1;
    if (n == 0) return (seq_t)0;
    while (nt < (size_t)n) {
        uint64_t m = next_m++;
        if (A084112_check(m)) {
            if (nt == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[nt++] = m;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
