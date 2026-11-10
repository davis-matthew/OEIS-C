/*
 * A080919  Numbers that do not appear in A080900.
 * https://oeis.org/A080919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   complement of A080900 (a(1)=1; a(n)=a(n-1)-2 if n already
 *            occurs, else a(n-1)+5); computed by simulating A080900 up to a
 *            limit and listing the unmarked integers
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A080919_comp = NULL;
static size_t A080919_cnt = 0;

static void A080919_build(uint64_t L)
{
    unsigned char *mark = mem_new(unsigned char, (size_t)L + 2);
    uint64_t *list = mem_new(uint64_t, (size_t)L + 2);
    int64_t prev = 1;
    uint64_t i, c = 0;
    mark[1] = 1;
    for (i = 2; i <= L; i++) {
        if (mark[i]) prev -= 2; else prev += 5;
        if (prev >= 1 && (uint64_t)prev <= L) mark[prev] = 1;
    }
    for (i = 1; i <= L; i++)
        if (!mark[i]) list[c++] = i;
    if (A080919_comp) mem_free(A080919_comp);
    mem_free(mark);
    A080919_comp = list;
    A080919_cnt = (size_t)c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t L = 64;
    while (n > A080919_cnt) {
        L = (uint64_t)n * 2 + 64;
        if (L < 2 * (uint64_t)(A080919_cnt ? A080919_cnt : 32)) L = 2 * (uint64_t)A080919_cnt;
        A080919_build(L);
        if (n <= A080919_cnt) break;
        L *= 2;
        A080919_build(L);
    }
    return (seq_t)A080919_comp[n - 1];
}
