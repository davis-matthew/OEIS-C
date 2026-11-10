/*
 * A083454  Positions of records in A084841.
 * https://oeis.org/A083454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k such that A084841(k) > A084841(j) for all j < k
 *            (records of A084841, found by scanning rows and counting
 *            deficient numbers)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A083454_sigma(uint64_t m)
{
    uint64_t s = 1, p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
            }
            s *= term;
        }
    }
    if (m > 1) s *= (1 + m);
    return s;
}

static uint64_t *A083454_rec = 0;
static uint64_t A083454_cnt = 0, A083454_cap = 0;
static uint64_t A083454_row = 0, A083454_best = 0;

seq_t a(idx_t n)
{
    while (A083454_cnt < (uint64_t)n) {
        uint64_t k = ++A083454_row;
        uint64_t start = k * (k - 1) / 2 + 1;
        uint64_t def = 0, i;
        for (i = 0; i < k; ++i) {
            uint64_t m = start + i;
            if (A083454_sigma(m) < 2 * m) ++def;
        }
        if (def > A083454_best) {
            A083454_best = def;
            if (A083454_cnt == A083454_cap) {
                A083454_cap = A083454_cap ? A083454_cap * 2 : 64;
                A083454_rec = (uint64_t *)realloc(A083454_rec, A083454_cap * sizeof(uint64_t));
            }
            A083454_rec[A083454_cnt++] = k;
        }
    }
    return (seq_t)A083454_rec[(uint64_t)n - 1];
}
