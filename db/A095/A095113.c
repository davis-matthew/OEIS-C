/*
 * A095113  a(1)=1. a(n) is the sum of n/d over all divisors d of n which are
 *          among a(1), ..., a(n-1).
 * https://oeis.org/A095113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of n/d over all divisors d of n that have already *
 *            appeared in the sequence. The sequence is defined by that rule
 *            over * positive indices, with a(1)=1. This implementation
 *            memoizes all * computed values in a dynamic array, maintains a
 *            sorted copy for fast * membership tests, and uses...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstddef>
#else
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#endif

/* Factorisation helper -----------------------------------------------------*/
typedef struct { uint32_t p; int e; } Factor;

static inline void build_spf(size_t limit, uint32_t **spf_out, size_t *spf_limit_out) {
    uint32_t *new_spf = (uint32_t *)realloc(*spf_out, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(1);
    for (size_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    if (new_spf[(size_t)j] == 0)
                        new_spf[(size_t)j] = (uint32_t)i;
            }
        }
    }
    if (limit >= 1) new_spf[1] = 1;
    *spf_out = new_spf;
    *spf_limit_out = limit;
}

static inline void ensure_spf(size_t limit, uint32_t **spf, size_t *spf_limit) {
    if (*spf_limit < limit) build_spf(limit, spf, spf_limit);
}

static inline void factorize(uint32_t n, Factor* fac, int* cnt, uint32_t *spf, size_t spf_limit) {
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        int e = 0;
        while (n % p == 0) { n /= p; ++e; }
        fac[(*cnt)++] = (Factor){p, e};
    }
}

/* C‑only global arrays for memoisation ------------------------------------ */
#ifndef __cplusplus
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static seq_t *sorted_vals_ptr = NULL;   /* sorted copy for membership tests */
static size_t sorted_len = 0;
static size_t sorted_cap = 0;
#endif

/* Helper to keep the sorted array updated --------------------------------- */
#ifndef __cplusplus
static inline void ensure_sorted_cap(size_t need) {
    if (sorted_cap < need) {
        size_t new_cap = sorted_cap ? sorted_cap << 1 : 4;
        if (new_cap < need) new_cap = need;
        sorted_vals_ptr = (seq_t *)realloc(sorted_vals_ptr, new_cap * sizeof(seq_t));
        sorted_cap = new_cap;
    }
}

static inline void insert_sorted(seq_t v) {
    ensure_sorted_cap(sorted_len + 1);
    size_t pos = sorted_len;
    while (pos > 0 && sorted_vals_ptr[pos - 1] > v)
        pos--;
    memmove(&sorted_vals_ptr[pos + 1], &sorted_vals_ptr[pos],
            (sorted_len - pos) * sizeof(seq_t));
    sorted_vals_ptr[pos] = v;
    sorted_len++;
}
#endif

/* Recursive divisor generator ---------------------------------------------*/
static inline void rec_divs(const Factor *fac, int fc, int idx, uint64_t cur,
                            uint32_t n, seq_t *sum
#ifdef __cplusplus
                            , const std::vector<seq_t> &sorted
#endif
                           )
{
    if (idx == fc) {
#ifdef __cplusplus
        auto it = std::lower_bound(sorted.begin(), sorted.end(),
                                   (seq_t)cur);
        if (it != sorted.end() && *it == (seq_t)cur)
            *sum += (seq_t)n / (seq_t)cur;
#else
        size_t lo = 0, hi = sorted_len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (sorted_vals_ptr[mid] < (seq_t)cur)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo < sorted_len && sorted_vals_ptr[lo] == (seq_t)cur)
            *sum += (seq_t)n / (seq_t)cur;
#endif
        return;
    }
    uint64_t prod = 1;
    for (int e = 0; e <= fac[idx].e; ++e) {
        rec_divs(fac, fc, idx + 1, cur * prod, n, sum
#ifdef __cplusplus
                , sorted
#endif
                );
        prod *= fac[idx].p;
    }
}

/* Main sequence function ---------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    static uint32_t *spf = NULL;
    static size_t spf_limit = 0;
    ensure_spf((size_t)n, &spf, &spf_limit);

#ifdef __cplusplus
    static std::vector<seq_t> seq_vals;
    static std::vector<seq_t> sorted_vals;

    if (!seq_vals.size()) {
        seq_vals.reserve(n);
        sorted_vals.reserve(n);
    }

    if (seq_vals.size() < (size_t)n) {
        for (size_t i = seq_vals.size(); i < (size_t)n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else {
                Factor fac[32];
                int fac_cnt;
                factorize((uint32_t)(i + 1), fac, &fac_cnt, spf, spf_limit);
                seq_t sum = 0;
                rec_divs(fac, fac_cnt, 0, 1, (uint32_t)(i + 1), &sum, sorted_vals);
                val = sum;
            }
            seq_vals.push_back(val);
            sorted_vals.push_back(val);
            std::inplace_merge(sorted_vals.begin(),
                               sorted_vals.end() - 1,
                               sorted_vals.end());
        }
    }
    return seq_vals[(size_t)n - 1];
#else
    if (seq_len < (size_t)n) {
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else {
                Factor fac[32];
                int fac_cnt;
                factorize((uint32_t)i, fac, &fac_cnt, spf, spf_limit);
                seq_t sum = 0;
                rec_divs(fac, fac_cnt, 0, 1, (uint32_t)i, &sum);
                val = sum;
            }

            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 4;
                seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq_arr[seq_len++] = val;
            insert_sorted(val);
        }
    }
    return seq_arr[(size_t)n - 1];
#endif
}
