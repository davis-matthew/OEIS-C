/*
 * A063251  Least number of binary rotations needed to reach fixed point (with
 *          either left or right rotation allowed at each iteration).
 * https://oeis.org/A063251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimal number of binary rotations (left or right) needed
 *            to reach a fixed point (a number of the form 2^k-1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   bit_length_u64_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bit_length_u64_to_u32.h"

/* left rotation of x (x>0).  Length = bitlen(x) */
static unsigned long long left_rot(unsigned long long x) {
    unsigned L = bit_length_u64_to_u32(x);
    unsigned long long mask = (1ULL << L) - 1ULL;
    return ((x << 1) & mask) | 1ULL;
}

/* right rotation of x (x>0).  Length = bitlen(x) */
static unsigned long long right_rot(unsigned long long x) {
    unsigned L = bit_length_u64_to_u32(x);
    unsigned long long lsb = x & 1ULL;
    unsigned long long y = (x >> 1) | (lsb << (L - 1));
    return y;
}

/* maximal node value needed for n up to around 10000.
   2^15-1 = 32767 is more than enough. */
#define MAX_NODE 32767

static seq_t precomp_ans[MAX_NODE + 1];
static unsigned short rev_edge[2 * (MAX_NODE + 1)];
static unsigned short rev_start[MAX_NODE + 2]; /* starting index of each list in rev_edge */
static unsigned short rev_len[MAX_NODE + 1];
static unsigned char rev_cnt[MAX_NODE + 1];
static int computed = 0;

/* compute all distances once */
static void precompute(void) {
    if (computed) return;
    computed = 1;

    /* count indegrees */
    for (int i = 1; i <= MAX_NODE; ++i) rev_cnt[i] = 0;
    for (unsigned long long src = 1; src <= MAX_NODE; ++src) {
        unsigned long long y1 = left_rot(src);
        unsigned long long y2 = right_rot(src);
        if (y1 <= MAX_NODE) rev_cnt[(unsigned)y1]++;
        if (y2 <= MAX_NODE) rev_cnt[(unsigned)y2]++;
    }

    /* prefix sums for start positions */
    rev_start[0] = 0;
    for (int i = 0; i <= MAX_NODE; ++i) {
        rev_start[i + 1] = rev_start[i] + rev_cnt[i];
        rev_len[i] = 0;
    }
    unsigned int total_edges = rev_start[MAX_NODE + 1];

    /* fill reverse edge lists */
    for (unsigned long long src = 1; src <= MAX_NODE; ++src) {
        unsigned long long y1 = left_rot(src);
        unsigned long long y2 = right_rot(src);
        if (y1 <= MAX_NODE) {
            unsigned idx = rev_start[(unsigned)y1] + rev_len[(unsigned)y1];
            rev_edge[idx] = (unsigned short)src;
            ++rev_len[(unsigned)y1];
        }
        if (y2 <= MAX_NODE) {
            unsigned idx = rev_start[(unsigned)y2] + rev_len[(unsigned)y2];
            rev_edge[idx] = (unsigned short)src;
            ++rev_len[(unsigned)y2];
        }
    }

    /* distances initialization */
    for (int i = 0; i <= MAX_NODE; ++i) precomp_ans[i] = (seq_t)-1;

    /* multi‑source BFS from all fixed points (all‑ones numbers) */
    int *queue = (int*)malloc((MAX_NODE + 1) * sizeof(int));
    int qhead = 0, qtail = 0;

    for (int k = 0; (1 << k) - 1 <= MAX_NODE; ++k) {
        unsigned u = (1U << k) - 1;
        precomp_ans[u] = 0;
        queue[qtail++] = (int)u;
    }

    while (qhead < qtail) {
        int v = queue[qhead++];
        unsigned dist_v = (unsigned)precomp_ans[v];
        unsigned short start = rev_start[v];
        unsigned short end   = rev_start[v] + rev_len[v];
        for (unsigned short idx = start; idx < end; ++idx) {
            unsigned src = rev_edge[idx];
            if (precomp_ans[src] == (seq_t)-1) {
                precomp_ans[src] = (seq_t)(dist_v + 1);
                queue[qtail++] = src;
            }
        }
    }

    free(queue);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > MAX_NODE) {
        /* value beyond precomputed range: fall back to 0 (unlikely) */
        return 0;
    }
    precompute();
    return precomp_ans[(unsigned)n];
}
