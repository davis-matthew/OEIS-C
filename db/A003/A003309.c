/*
 * A003309  Ludic numbers: apply the same sieve as Eratosthenes, but cross off
 *          every k-th remaining number.
 * https://oeis.org/A003309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve, not a closed formula. Start with 2,3,4,... Repeatedly
 *            take the first remaining number k (a ludic number), remove it,
 *            then remove every k-th of the numbers left. a(1) = 1. A Fenwick
 *            tree finds the j-th remaining number quickly
 * Range:     u8 n <= 48, u16 n <= 5450, u32 n <= 9497, u64 n <= 9497,
 *            u128 n <= 9497, big any n
 * Helpers:   fw_dec, fw_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/fw_dec.h"
#include "../utils/fw_find.h"

static uint64_t *lud_vals = NULL;
static size_t lud_cnt = 0;
static uint64_t lud_built = 0;

static void lud_build(uint64_t L)
{
    size_t m = (size_t)(L - 1); /* positions 1..m hold values 2..L */
    uint32_t *t = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    uint64_t *out = (uint64_t *)malloc((m + 2) * sizeof(uint64_t));
    size_t i, top, cnt = 0;
    uint64_t rem = m;

    t[0] = 0;
    for (i = 1; i <= m; i++) t[i] = (uint32_t)(i & (~i + 1));
    top = 1;
    while (top * 2 <= m) top *= 2;

    out[cnt++] = 1;
    while (rem > 0) {
        size_t p = fw_find(t, m, top, 1);
        uint64_t k = (uint64_t)p + 1;
        uint64_t idx;
        out[cnt++] = k;
        fw_dec(t, m, p);
        rem--;
        idx = k;
        while (idx <= rem) {
            size_t q = fw_find(t, m, top, idx);
            fw_dec(t, m, q);
            rem--;
            idx += k - 1;
        }
    }
    free(t);
    free(lud_vals);
    lud_vals = out;
    lud_cnt = cnt;
    lud_built = L;
}

seq_t a(idx_t n)
{
    while ((idx_t)lud_cnt < n) {
        uint64_t L = (uint64_t)n * 20 + 100;
        if (L < lud_built * 2) L = lud_built * 2;
        lud_build(L);
    }
    return (seq_t)lud_vals[n - 1];
}
