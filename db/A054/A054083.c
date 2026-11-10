/*
 * A054083  a(n) = order of n in the permutation A054082 of the natural
 *          numbers if this order exists; a(n) = -1 otherwise.
 * https://oeis.org/A054083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = length of the cycle of n under the permutation A054082 (i
 *            -> A054082(i)), or -1 if the orbit does not return to n within a
 *            large computed range (search by iteration)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A054083_perm = NULL;
static const uint32_t A054083_M = 1u << 21;

static void A054083_init(void)
{
    if (A054083_perm) return;
    uint32_t M = A054083_M;
    uint32_t *arr = (uint32_t *)calloc((size_t)M + 2, sizeof(uint32_t));
    unsigned char *seen = (unsigned char *)calloc((size_t)2 * M + 16, 1);
    if (!arr || !seen) abort();
    arr[1] = 2; arr[2] = 1;
    seen[1] = 1; seen[2] = 1;
    uint32_t p = 3;
    for (uint32_t k = 2; 2 * k <= M; k++) {
        while (seen[p]) p++;
        uint32_t q = p + k - 1;
        arr[2 * k - 1] = q;
        arr[2 * k] = p;
        seen[p] = 1;
        seen[q] = 1;
    }
    free(seen);
    A054083_perm = arr;
}

seq_t a(idx_t n)
{
    A054083_init();
    if (n < 1 || n > A054083_M) return (seq_t)(-1);
    uint32_t start = (uint32_t)n;
    uint32_t cur = start;
    uint32_t len = 0;
    while (true) {
        if (cur > A054083_M || cur == 0) return (seq_t)(-1);
        cur = A054083_perm[cur];
        len++;
        if (cur == start) return (seq_t)len;
        if (len > A054083_M) return (seq_t)(-1);
    }
}
