/*
 * A003387  Numbers that are the sum of 9 nonzero 8th powers.
 * https://oeis.org/A003387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all nondecreasing 9-tuples of positive bases
 *            whose 8th powers sum to at most a bound, sort and deduplicate
 *            the sums; the bound is doubled until at least n distinct sums
 *            are found
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3387_cmpu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/A3387_cmpu.h"

static uint64_t *A3387_buf = NULL;
static size_t A3387_len = 0, A3387_cap = 0;
static uint64_t A3387_p8[256];
static int A3387_nb = 0;

static uint64_t *A3387_vals = NULL;
static idx_t A3387_cnt = 0;
static uint64_t A3387_lim = 0;

static void A3387_push(uint64_t v) {
    if (A3387_len == A3387_cap) {
        size_t nc = A3387_cap ? A3387_cap * 2 : 1024;
        A3387_buf = (uint64_t *)realloc(A3387_buf, nc * sizeof(uint64_t));
        A3387_cap = nc;
    }
    A3387_buf[A3387_len++] = v;
}

static void A3387_rec(int rem, int minb, uint64_t sum, uint64_t limit) {
    int b;
    if (rem == 0) {
        A3387_push(sum);
        return;
    }
    for (b = minb; b < A3387_nb; b++) {
        uint64_t p = A3387_p8[b];
        /* remaining terms are all >= p, need sum + rem*p <= limit */
        if (p > (limit - sum) / (uint64_t)rem) break;
        A3387_rec(rem - 1, b, sum + p, limit);
    }
}

static void A3387_build(uint64_t limit) {
    size_t i, k;
    if (A3387_nb == 0) {
        int b;
        for (b = 1; b < 256; b++) {
            uint64_t p = 1;
            int e;
            for (e = 0; e < 8; e++) p *= (uint64_t)b;
            A3387_p8[b] = p;
            A3387_nb = b + 1;
            if (b >= 180) break; /* 180^8 > 2^59 */
        }
    }
    A3387_len = 0;
    A3387_rec(9, 1, 0, limit);
    qsort(A3387_buf, A3387_len, sizeof(uint64_t), A3387_cmpu);
    k = 0;
    for (i = 0; i < A3387_len; i++)
        if (k == 0 || A3387_buf[i] != A3387_buf[k - 1]) A3387_buf[k++] = A3387_buf[i];
    free(A3387_vals);
    A3387_vals = (uint64_t *)malloc((k ? k : 1) * sizeof(uint64_t));
    memcpy(A3387_vals, A3387_buf, k * sizeof(uint64_t));
    A3387_cnt = (idx_t)k;
    A3387_lim = limit;
}

seq_t a(idx_t n) {
    uint64_t limit;
    if (n < 1) return (seq_t)0;
    limit = A3387_lim ? A3387_lim : (uint64_t)1 << 20;
    while (A3387_cnt < n || A3387_vals == NULL) {
        if (A3387_vals != NULL) {
            if (limit >= ((uint64_t)1 << 58)) break;
            limit *= 2;
        }
        A3387_build(limit);
    }
    if (n > A3387_cnt) return (seq_t)0;
    return (seq_t)A3387_vals[n - 1];
}
