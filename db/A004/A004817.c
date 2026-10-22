/*
 * A004817  Numbers that are the sum of 6 positive 11th powers.
 * https://oeis.org/A004817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: enumerate all sums of 6 positive 11th powers with bases
 *            <= B, keep those below (B+1)^11 + 5 (which are then complete),
 *            sort, dedupe; increase B until n terms are known
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 784, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *cached_terms = NULL;
static idx_t cached_count = 0;
static idx_t current_max_base = 0;

static u128 pow11_u(idx_t base) {
    u128 result = (u128)base;
    for (int i = 0; i < 10; i++) result *= (u128)base;
    return result;
}

static int compare_u128(const void *x, const void *y) {
    u128 va = *(const u128 *)x;
    u128 vb = *(const u128 *)y;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void generate_with_base(idx_t max_base) {
    idx_t est = 1;
    for (idx_t i = 0; i < 6; i++) {
        est = est * (max_base + 5 - i) / (i + 1);
    }

    u128 *p = (u128 *)malloc(sizeof(u128) * (max_base + 2));
    for (idx_t i = 1; i <= max_base + 1; i++) p[i] = pow11_u(i);
    u128 limit = p[max_base + 1] + 5;

    u128 *temp = (u128 *)malloc(sizeof(u128) * (est + 1));
    idx_t count = 0;

    for (idx_t a = 1; a <= max_base; a++) {
        u128 sa = p[a];
        for (idx_t b = a; b <= max_base; b++) {
            u128 sb = sa + p[b];
            for (idx_t c = b; c <= max_base; c++) {
                u128 sc = sb + p[c];
                for (idx_t d = c; d <= max_base; d++) {
                    u128 sd = sc + p[d];
                    for (idx_t e = d; e <= max_base; e++) {
                        u128 se = sd + p[e];
                        for (idx_t f = e; f <= max_base; f++) {
                            u128 s = se + p[f];
                            if (s < limit) temp[count++] = s;
                        }
                    }
                }
            }
        }
    }
    free(p);

    qsort(temp, (size_t)count, sizeof(u128), compare_u128);

    idx_t unique_count = 0;
    for (idx_t i = 0; i < count; i++) {
        if (i == 0 || temp[i] != temp[i - 1]) {
            temp[unique_count++] = temp[i];
        }
    }

    free(cached_terms);
    cached_terms = temp;
    cached_count = unique_count;
    current_max_base = max_base;
}

seq_t a(idx_t n) {
    while (cached_count < n) {
        generate_with_base(current_max_base + 1);
    }
    return u128_to_seq(cached_terms[n - 1]);
}
