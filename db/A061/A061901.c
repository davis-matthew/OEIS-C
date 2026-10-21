/*
 * A061901  Number of distinct numbers that can be formed from the digits of
 *          n.
 * https://oeis.org/A061901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct numbers that can be formed from the
 *            digits of n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#include <functional>
#endif

/* Count the occurrence of each decimal digit 0..9 in n. */
static void digit_counts(idx_t n, unsigned char cnt[10]) {
    for (int i = 0; i < 10; ++i) cnt[i] = 0;
    if (n == 0) {
        cnt[0] = 1;
        return;
    }
    while (n > 0) {
        unsigned d = n % 10;
        ++cnt[d];
        n /= 10;
    }
}

#ifndef __cplusplus  /* C implementation of a dynamic array */
struct seq_arr {
    seq_t *data;
    size_t len;
    size_t cap;
};

static void arr_init(struct seq_arr *arr, size_t initial_cap) {
    arr->data = (seq_t *)malloc(initial_cap * sizeof(seq_t));
    arr->len = 0;
    arr->cap = initial_cap;
}

static void arr_push(struct seq_arr *arr, seq_t val) {
    if (arr->len == arr->cap) {
        size_t new_cap = arr->cap ? arr->cap * 2 : 16;
        seq_t *new_data = (seq_t *)realloc(arr->data, new_cap * sizeof(seq_t));
        if (!new_data) return; /* allocation failed, ignore (will crash) */
        arr->data = new_data;
        arr->cap = new_cap;
    }
    arr->data[arr->len++] = val;
}

static void arr_free(struct seq_arr *arr) {
    free(arr->data);
}

/* Comparator for qsort */
static int seq_cmp(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}
#endif

/* Recursive generation of all numbers obtainable from the digits of n. */
static void dfs_gen(unsigned char cnt[10], seq_t current,
                    /* in C++: std::vector<seq_t> *out
                       in C:     struct seq_arr    *out */
                    void *out_ptr) {
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] == 0) continue;
        --cnt[d];
        seq_t next = current * 10 + d;
#ifdef __cplusplus
        std::vector<seq_t> *vec = (std::vector<seq_t> *)out_ptr;
        vec->push_back(next);
#else
        struct seq_arr *arr = (struct seq_arr *)out_ptr;
        arr_push(arr, next);
#endif
        dfs_gen(cnt, next, out_ptr);
        ++cnt[d];
    }
}

seq_t a(idx_t n) {
    unsigned char cnt[10];
    digit_counts(n, cnt);

    seq_t result = 0;

#ifdef __cplusplus
    std::vector<seq_t> numbers;
    numbers.reserve(512);   /* adequate for n <= 10000 */
    unsigned char cnt_copy[10];
    memcpy(cnt_copy, cnt, sizeof(cnt));
    dfs_gen(cnt_copy, 0, (void *)&numbers);
    std::sort(numbers.begin(), numbers.end(),
              [](const seq_t &a, const seq_t &b) { return a < b; });
    size_t unique_cnt = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (i == 0 || numbers[i] != numbers[i - 1]) ++unique_cnt;
    }
    result = (seq_t)unique_cnt;
#else
    struct seq_arr arr;
    arr_init(&arr, 16);
    unsigned char cnt_copy[10];
    memcpy(cnt_copy, cnt, sizeof(cnt));
    dfs_gen(cnt_copy, 0, (void *)&arr);
    /* sort the generated numbers */
    if (arr.len > 0) {
        qsort(arr.data, arr.len, sizeof(seq_t), seq_cmp);
        /* count distinct values */
        size_t unique_cnt = 1;
        for (size_t i = 1; i < arr.len; ++i)
            if (arr.data[i] != arr.data[i - 1]) ++unique_cnt;
        result = (seq_t)unique_cnt;
    } else {
        /* should not happen, but keep result 0 */
        result = (seq_t)0;
    }
    arr_free(&arr);
#endif
    return result;
}
