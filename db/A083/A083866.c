/*
 * A083866  Positions of zeros in Per Nørgård's infinity sequence (A004718).
 * https://oeis.org/A083866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 0 with A004718(m) = 0, where A004718(2m) =
 *            -A004718(m), A004718(2m+1) = A004718(m)+1, A004718(0) = 0;
 *            computed by a doubling scan
 * Range:     u8 n <= 34, u16 n <= 6434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9839 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A083866_z = NULL;
static uint64_t A083866_cnt = 0;
static uint64_t A083866_L = 0;

static void A083866_build(uint64_t L) {
    int *v = (int *)malloc(L * sizeof(int));
    free(A083866_z);
    A083866_z = (uint64_t *)malloc(L * sizeof(uint64_t));
    A083866_cnt = 0;
    for (uint64_t i = 0; i < L; i++) {
        if (i == 0) v[i] = 0;
        else if (i & 1) v[i] = v[i >> 1] + 1;
        else v[i] = -v[i >> 1];
        if (v[i] == 0) A083866_z[A083866_cnt++] = i;
    }
    free(v);
    A083866_L = L;
}

seq_t a(idx_t n) {
    if (A083866_L == 0) A083866_build((uint64_t)1 << 16);
    while (A083866_cnt <= (uint64_t)n) A083866_build(A083866_L * 2);
    return (seq_t)A083866_z[n];
}
