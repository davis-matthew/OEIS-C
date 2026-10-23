/*
 * A123295  Sum of 14 positive 5th powers.
 * https://oeis.org/A123295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth smallest integer that can be expressed as a sum of 14
 *            positive fifth powers. The sequence is computed by enumerating
 *            all multisets of 14 indices from 1..M (M = 10) and collecting
 *            their sums
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- dynamic array for seq_t values ---------- */
typedef struct {
    seq_t *data;
    size_t size;
    size_t capacity;
} SeqArray;

/* ensure the array has room for at least 'need' elements */
static void ensure_capacity(SeqArray *a, size_t need) {
    if (need <= a->capacity)
        return;
    size_t new_cap = a->capacity ? a->capacity : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_data = (seq_t *)realloc(a->data, new_cap * sizeof(seq_t));
    a->data = new_data;
    a->capacity = new_cap;
}

static void push_back(SeqArray *a, seq_t val) {
    ensure_capacity(a, a->size + 1);
    a->data[a->size++] = val;
}

/* ---------- recursion generating all multisets of size 14 ---------- */
static void dfs_rec(int pos, int rem, seq_t current_sum,
                    const seq_t *powers, int M, SeqArray *out) {
    if (pos > M) {
        if (rem == 0)
            push_back(out, current_sum);
        return;
    }
    /* choose how many of the current exponent (pos) are used (0..rem) */
    for (int cnt = 0; cnt <= rem; ++cnt) {
        seq_t new_sum = current_sum + (seq_t)cnt * powers[pos];
        dfs_rec(pos + 1, rem - cnt, new_sum, powers, M, out);
    }
}

/* comparator for qsort, works for both integral and arbitrary-precision seq_t */
static int cmp_seq_t(const void *a1, const void *a2) {
    seq_t v1 = *(const seq_t *)a1;
    seq_t v2 = *(const seq_t *)a2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* ---------- global cache ---------- */
static SeqArray g_seq = {NULL, 0, 0};
static bool g_inited = false;

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (!g_inited) {
        /* precompute powers of 1..M */
        const int M = 10;          /* number of distinct fifth powers used */
        seq_t powers[M + 1];
        for (int i = 1; i <= M; ++i) {
            seq_t p = 1;
            for (int e = 0; e < 5; ++e)
                p *= (seq_t)i;
            powers[i] = p;
        }

        /* generate all sums of 14 numbers from these powers */
        SeqArray all = {NULL, 0, 0};
        dfs_rec(1, 14, 0, powers, M, &all);

        /* sort and deduplicate */
        qsort(all.data, all.size, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < all.size; ++i) {
            if (i == 0 || all.data[i] != all.data[i - 1]) {
                all.data[uniq++] = all.data[i];
            }
        }
        all.size = uniq;

        /* store in global cache */
        g_seq.data = all.data;
        g_seq.size = all.size;
        g_seq.capacity = all.capacity;
        all.data = NULL;  /* ownership transferred */
        g_inited = true;
    }

    /* offset is 1, so a(1) corresponds to element 0 */
    /* Caller guarantees that n is within the precomputed range. */
    return g_seq.data[n - 1];
}
