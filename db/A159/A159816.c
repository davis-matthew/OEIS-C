/*
 * A159816  Seven-digit terms in A023086.
 * https://oeis.org/A159816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 288, u64 n <= 288, u128 n <= 288,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159816 – Seven‑digit terms in A023086.
 * The sequence consists of the 7‑digit integers n such that
 *  n and 2*n each contain 7 distinct digits chosen from one of
 *  the sets {0,1,2,4,5,7,8} or {1,2,4,5,7,8,9},
 *  and the two numbers have exactly the same digit set.
 * The terms are listed in increasing order. */

/* Standard headers. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Local helper: lexicographic next permutation (C standard algorithm). */
static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        l++; r--;
    }
    return true;
}

/* Comparison routine for qsort. */
static int compare_seq(const void *p1, const void *p2)
{
    seq_t x = *(const seq_t*)p1;
    seq_t y = *(const seq_t*)p2;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Static cache of the sequence. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static bool seq_built = false;

/* Build the complete sequence into seq_cache. */
static void build_sequence(void)
{
    /* The two digit sets. */
    const int set1[7] = {0, 1, 2, 4, 5, 7, 8};
    const int set2[7] = {1, 2, 4, 5, 7, 8, 9};

    /* The maximum number of permutations per set: 7! = 5040. */
    const size_t max_perms = 5040;
    const size_t temp_capacity = 2 * max_perms;

    /* Allocate temporary storage for all candidate 7‑digit numbers. */
    seq_t *temp = (seq_t*)malloc(temp_capacity * sizeof(seq_t));
    if (!temp) return;          /* Allocation failed – will leave cache empty */
    size_t temp_cnt = 0;

    /* Generate permutations of the first set (skip those with leading zero). */
    int perm[7];
    memcpy(perm, set1, 7 * sizeof(int));
    do {
        if (perm[0] != 0) {
            seq_t v = 0;
            for (int k = 0; k < 7; ++k) v = v * 10 + (seq_t)perm[k];
            temp[temp_cnt++] = v;
        }
    } while (next_permutation(perm, 7));

    /* Generate permutations of the second set (all are valid). */
    memcpy(perm, set2, 7 * sizeof(int));
    do {
        seq_t v = 0;
        for (int k = 0; k < 7; ++k) v = v * 10 + (seq_t)perm[k];
        temp[temp_cnt++] = v;
    } while (next_permutation(perm, 7));

    /* Temporary array for valid terms. */
    seq_t *valid = (seq_t*)malloc(temp_cnt * sizeof(seq_t));
    if (!valid) { free(temp); return; }
    size_t valid_cnt = 0;

    /* Test each candidate. */
    for (size_t i = 0; i < temp_cnt; ++i) {
        seq_t n = temp[i];
        seq_t m = n * 2;
        if (m >= 10000000) continue;          /* must stay 7‑digit */

        /* Compute bit masks of digits; assumed n has distinct digits by construction. */
        seq_t mask_n = 0;
        seq_t t = n;
        for (int k = 0; k < 7; ++k) {
            int d = (int)(t % 10);
            mask_n |= ((seq_t)1 << d);
            t /= 10;
        }

        /* Compute mask for m, ensuring digits are distinct. */
        seq_t mask_m = 0;
        seq_t s = m;
        bool dup = false;
        for (int k = 0; k < 7; ++k) {
            int d = (int)(s % 10);
            if (mask_m & ((seq_t)1 << d)) { dup = true; break; }
            mask_m |= ((seq_t)1 << d);
            s /= 10;
        }
        if (dup) continue;
        if (mask_n == mask_m) valid[valid_cnt++] = n;
    }

    free(temp);

    /* Sort the result. */
    qsort(valid, valid_cnt, sizeof(seq_t), compare_seq);

    /* Store in static cache. */
    seq_cache = valid;
    seq_len   = valid_cnt;
    seq_built = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at index 1 */
    if (!seq_built) build_sequence();
    if (n <= seq_len) return seq_cache[n - 1];
    return 0;                      /* undefined for indices beyond the known terms */
}
