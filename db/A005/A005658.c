/*
 * A005658  If n appears so do 2n, 3n+2, 6n+3.
 * https://oeis.org/A005658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve -- 1 is in the set, and if m is in it so are 2m, 3m+2 and
 *            6m+3; a(n) is the n-th smallest member (the sieve limit doubles
 *            until n members are found)
 * Range:     u8 n <= 126, u16 n <= 9699, u32 n <= 9699, u64 n <= 9699,
 *            u128 n <= 9699, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a005658_terms = NULL;
static uint64_t a005658_count = 0;
static uint64_t a005658_limit = 0;

static void a005658_build(uint64_t limit)
{
    uint8_t *in_set = (uint8_t *)calloc((size_t)(limit / 8 + 1), 1);
    uint64_t i, cnt = 0;
    uint64_t *terms;
    if (!in_set) return;
    in_set[0] |= (uint8_t)(1u << 1);
    for (i = 1; i < limit; i++) {
        if ((in_set[i >> 3] >> (i & 7)) & 1) {
            uint64_t j1 = 2 * i, j2 = 3 * i + 2, j3 = 6 * i + 3;
            if (j1 < limit) in_set[j1 >> 3] |= (uint8_t)(1u << (j1 & 7));
            if (j2 < limit) in_set[j2 >> 3] |= (uint8_t)(1u << (j2 & 7));
            if (j3 < limit) in_set[j3 >> 3] |= (uint8_t)(1u << (j3 & 7));
            cnt++;
        }
    }
    terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    if (!terms) { free(in_set); return; }
    cnt = 0;
    for (i = 1; i < limit; i++)
        if ((in_set[i >> 3] >> (i & 7)) & 1)
            terms[cnt++] = i;
    free(in_set);
    free(a005658_terms);
    a005658_terms = terms;
    a005658_count = cnt;
    a005658_limit = limit;
}

seq_t a(idx_t n)
{
    uint64_t limit;
    if (n < 1) return 0;
    limit = a005658_limit ? a005658_limit : 1024;
    while (a005658_count < n) {
        if (a005658_limit >= limit) limit *= 2;
        a005658_build(limit);
        if (a005658_limit != limit) return 0;
    }
    return (seq_t)a005658_terms[n - 1];
}
