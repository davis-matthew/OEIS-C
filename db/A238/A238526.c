/*
 * A238526  Record values of A238525.
 * https://oeis.org/A238526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th strict record value of A238525(m) = m mod sopfr(m),
 *            m >= 2; computed by a sieve scan
 * Range:     u8 n <= 57, u16 n <= 6545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9791 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A238526_rec = 0;
static size_t A238526_cnt = 0;

static void A238526_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *sp = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t *rec = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    size_t cnt = 0;
    uint64_t i, j;
    int have = 0;
    uint64_t best = 0;

    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= L; i++) {
        uint64_t p = spf[i];
        sp[i] = (uint32_t)(sp[i / p] + p);
        uint64_t v = i % sp[i];
        if (!have || v > best) {
            have = 1;
            best = v;
            rec[cnt++] = v;
        }
    }
    free(spf);
    free(sp);
    free(A238526_rec);
    A238526_rec = rec;
    A238526_cnt = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t L = 1ULL << 14;
    if (n < 1)
        return 0;
    if (A238526_rec == 0)
        A238526_build(L);
    while ((uint64_t)n > A238526_cnt) {
        L *= 2;
        A238526_build(L);
    }
    return (seq_t)A238526_rec[n - 1];
}
