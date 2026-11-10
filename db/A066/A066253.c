/*
 * A066253  a(n) = A066249(A066249(n)).
 * https://oeis.org/A066253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066249(A066249(n)), where A066249(m) = prime(m/2) - 1
 *            for even m and composite((m+1)/2) - 1 for odd m (sieve-based)
 * Range:     u8 n <= 59, u16 n <= 3113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9117 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A066253_sv = NULL;   /* 1 = prime */
static uint64_t A066253_cap = 0;

static void A066253_ensure(uint64_t lim)
{
    if (A066253_cap >= lim) return;
    free(A066253_sv);
    A066253_sv = (unsigned char *)malloc((size_t)lim + 1);
    if (!A066253_sv) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= lim; i++) A066253_sv[i] = 1;
    A066253_sv[0] = 0;
    if (lim >= 1) A066253_sv[1] = 0;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (A066253_sv[i])
            for (uint64_t j = i * i; j <= lim; j += i) A066253_sv[j] = 0;
    A066253_cap = lim;
}

static uint64_t A066253_f(uint64_t m)
{
    if (m == 0) return 0;
    bool even = (m % 2 == 0);
    uint64_t k = even ? m / 2 : (m + 1) / 2;
    uint64_t lim = 2 * k + 1000;
    for (;;) {
        A066253_ensure(lim);
        uint64_t cnt = 0;
        for (uint64_t i = even ? 2 : 4; i <= A066253_cap; i++) {
            bool p = A066253_sv[i] != 0;
            if (p == even) {
                if (++cnt == k) return i - 1;
            }
        }
        lim = A066253_cap * 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t inner = A066253_f((uint64_t)n);
    return (seq_t)A066253_f(inner);
}
