/*
 * A217845  Numbers which are the sums of consecutive fifth powers.
 * https://oeis.org/A217845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the set of sums of consecutive *
 *            fifth powers. This implementation generates all sums up to a
 *            growing * maximum base until enough unique values are obtained,
 *            then stores the * sorted, deduplicated list in a static array.
 *            The function works for * both C11 and C++17 builds
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 23, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#else
#include <stdlib.h>
#endif

/* Helper: compute i^5 */
static inline seq_t pow5(idx_t i) {
    seq_t r = 1;
    for (int k = 0; k < 5; ++k)
        r *= i;
    return r;
}

#ifdef __cplusplus
/* C++ implementation using std::vector */
static struct {
    std::vector<seq_t> data;
    idx_t current_base;
    bool ready;
} cache_cpp = { std::vector<seq_t>(), 0, false };

static void rebuild_cache_cpp(idx_t n) {
    /* Increase the maximum base until we have at least n unique sums.
       We double the base each time to keep the algorithm O(n). */
    if (!cache_cpp.ready || cache_cpp.data.size() < (size_t)n) {
        idx_t base = cache_cpp.current_base;
        if (base == 0) base = 100;   /* initial guess */
        while (cache_cpp.data.size() < (size_t)n) {
            idx_t new_base = base * 2;
            /* Prefix sums of i^5 */
            std::vector<seq_t> pref(new_base + 1);
            pref[0] = 0;
            for (idx_t i = 1; i <= new_base; ++i)
                pref[i] = pref[i - 1] + pow5(i);

            /* Generate all consecutive sums */
            std::vector<seq_t> all;
            all.reserve((size_t)(new_base + 1) * (new_base + 2) / 2);
            for (idx_t i = 0; i <= new_base; ++i) {
                for (idx_t j = i; j <= new_base; ++j) {
                    seq_t sum = (i == 0) ? pref[j] : pref[j] - pref[i - 1];
                    all.push_back(sum);
                }
            }

            /* Sort and deduplicate */
            std::sort(all.begin(), all.end());
            auto it = std::unique(all.begin(), all.end());
            all.erase(it, all.end());

            cache_cpp.data.swap(all);
            base = new_base;
        }
        cache_cpp.current_base = base;
        cache_cpp.ready = true;
    }
}
#else
/* C implementation using malloc/qsort */
static seq_t *cache_c = NULL;
static size_t cache_len = 0;
static idx_t cache_current_base = 0;
static int cache_ready = 0;

static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void rebuild_cache_c(idx_t n) {
    if (!cache_ready || cache_len < (size_t)n) {
        idx_t base = cache_current_base;
        if (base == 0) base = 100;
        while (cache_len < (size_t)n) {
            idx_t new_base = base * 2;

            /* Prefix sums of i^5 */
            seq_t *pref = (seq_t *)malloc(sizeof(seq_t) * (new_base + 1));
            if (!pref) exit(1);
            pref[0] = 0;
            for (idx_t i = 1; i <= new_base; ++i)
                pref[i] = pref[i - 1] + pow5(i);

            /* Generate all consecutive sums */
            size_t total = (size_t)(new_base + 1) * (new_base + 2) / 2;
            seq_t *all = (seq_t *)malloc(sizeof(seq_t) * total);
            if (!all) exit(1);
            size_t idx = 0;
            for (idx_t i = 0; i <= new_base; ++i) {
                for (idx_t j = i; j <= new_base; ++j) {
                    seq_t sum = (i == 0) ? pref[j] : pref[j] - pref[i - 1];
                    all[idx++] = sum;
                }
            }

            /* Sort and deduplicate */
            qsort(all, total, sizeof(seq_t), cmp_seq);
            size_t unique_cnt = 0;
            for (size_t k = 0; k < total; ++k) {
                if (k == 0 || all[k] != all[k - 1]) unique_cnt++;
            }
            seq_t *new_cache = (seq_t *)realloc(cache_c, sizeof(seq_t) * unique_cnt);
            if (!new_cache) exit(1);
            cache_c = new_cache;

            size_t dst = 0;
            for (size_t k = 0; k < total; ++k) {
                if (k == 0 || all[k] != all[k - 1]) {
                    cache_c[dst++] = all[k];
                }
            }
            cache_len = unique_cnt;

            free(all);
            free(pref);
            base = new_base;
        }
        cache_current_base = base;
        cache_ready = 1;
    }
}
#endif

/* ******************************************************************** */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */
#ifdef __cplusplus
    rebuild_cache_cpp(n);
    return cache_cpp.data[(size_t)(n - 1)];
#else
    rebuild_cache_c(n);
    return cache_c[(size_t)(n - 1)];
#endif
}
