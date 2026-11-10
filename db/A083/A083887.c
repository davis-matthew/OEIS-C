/*
 * A083887  Positions of records in A085020.
 * https://oeis.org/A083887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th record (strict left-to-right
 *            maximum) in A085020, where A085020(m) = Sum_{p prime, (p-1)|m}
 *            p; computed by a doubling sieve
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *rec_pos = NULL;
static size_t rec_cnt = 0;

static void rec_build(uint64_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 2, 1);
    uint64_t *s = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    uint64_t p, m;
    for (p = 2; p <= L + 1; p++) {
        if (comp[p]) continue;
        for (m = p * p; m <= L + 1; m += p) comp[m] = 1;
        uint64_t d = p - 1;
        for (m = d; m <= L; m += d) s[m] += p;
    }
    free(rec_pos);
    rec_pos = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    rec_cnt = 0;
    uint64_t best = 0;
    for (m = 1; m <= L; m++) {
        if (s[m] > best) {
            best = s[m];
            rec_pos[rec_cnt++] = m;
        }
    }
    free(comp);
    free(s);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (n == 0) return 0;
    while (rec_cnt < (size_t)n) {
        limit = limit ? limit * 2 : 1024;
        rec_build(limit);
    }
    return (seq_t)rec_pos[(size_t)n - 1];
}
