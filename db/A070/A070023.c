/*
 * A070023  1/n has period 1 in base 10 (including terminating decimals).
 * https://oeis.org/A070023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth smallest integer of the form 3^e * 2^i * 5^j where e
 *            ∈ {0,1,2}, i ≥ 0, j ≥ 0. The sequence begins 1, 2, 3, 4, 5, 6,
 *            8, 9, 10, 12, 15, 16,
 * Range:     u8 n <= 18, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifndef __cplusplus
/* C implementation – seq_t is an unsigned integer type. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}
#else
/* C++ implementation – seq_t is an arbitrary‑precision integer type.
   std::vector and std::sort are used. */
#include <vector>
#include <algorithm>
#endif

/* Count how many numbers ≤ limit of the form 3^e * 2^i * 5^j,
   with e ∈ {0,1,2}.  The result fits in size_t for the ranges
   of n that we ever query. */
static size_t count_leq(seq_t limit)
{
    static const seq_t pow3[3] = { (seq_t)1, (seq_t)3, (seq_t)9 };
    size_t cnt = 0;
    for (int e = 0; e < 3; ++e) {
        seq_t factor3 = pow3[e];
        for (seq_t val2 = (seq_t)1; ; val2 *= (seq_t)2) {
            seq_t prod2 = factor3 * val2;
            if (prod2 > limit) break;
            for (seq_t val5 = (seq_t)1; ; val5 *= (seq_t)5) {
                seq_t val = prod2 * val5;
                if (val > limit) break;
                ++cnt;
            }
        }
    }
    return cnt;
}

/* Generate all numbers ≤ limit of the required form, storing them
   in the supplied array *arr (allocated within this function if
   necess. by C++ code, freed automatically in C).  len will
   hold the number of elements.  In C, cap will be set to the
   allocated capacity for caller sanity. */
static void generate_all(seq_t limit,
#ifdef __cplusplus
                          std::vector<seq_t> &v
#else
                          seq_t **arr, size_t *len, size_t *cap
#endif
                          )
{
    static const seq_t pow3[3] = { (seq_t)1, (seq_t)3, (seq_t)9 };

#ifdef __cplusplus
    /* C++: use vector, no extra memory handling here. */
    for (int e = 0; e < 3; ++e) {
        seq_t factor3 = pow3[e];
        for (seq_t val2 = (seq_t)1; ; val2 *= (seq_t)2) {
            seq_t prod2 = factor3 * val2;
            if (prod2 > limit) break;
            for (seq_t val5 = (seq_t)1; ; val5 *= (seq_t)5) {
                seq_t val = prod2 * val5;
                if (val > limit) break;
                v.push_back(val);
            }
        }
    }
#else
    /* C: allocate array dynamically. */
    for (int e = 0; e < 3; ++e) {
        seq_t factor3 = pow3[e];
        for (seq_t val2 = (seq_t)1; ; val2 *= (seq_t)2) {
            seq_t prod2 = factor3 * val2;
            if (prod2 > limit) break;
            for (seq_t val5 = (seq_t)1; ; val5 *= (seq_t)5) {
                seq_t val = prod2 * val5;
                if (val > limit) break;
                if (*len == *cap) {
                    size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
                    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* memory failure */
                    *arr = tmp;
                    *cap = new_cap;
                }
                (*arr)[(*len)++] = val;
            }
        }
    }
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safe guard – outside OEIS offset */

    /* 1. Find a limit that contains at least n numbers. */
    seq_t low  = (seq_t)1;
    seq_t high = (seq_t)1;
    while (count_leq(high) < (size_t)n) {
        high *= (seq_t)2;
    }

    /* 2. Binary search for the minimal limit with count >= n. */
    while (low < high) {
        seq_t mid = low + (high - low) / (seq_t)2;
        if (count_leq(mid) >= (size_t)n) {
            high = mid;
        } else {
            low = mid + (seq_t)1;
        }
    }
    seq_t limit = low;

    /* 3. Generate all qualified numbers <= limit and sort them. */
#ifdef __cplusplus
    std::vector<seq_t> vec;
    vec.reserve(count_leq(limit) + 5);
    generate_all(limit, vec);
    std::sort(vec.begin(), vec.end());
    return vec[n - 1];
#else
    size_t len = 0, cap = 0;
    seq_t *arr = NULL;
    generate_all(limit, &arr, &len, &cap);
    if (len < (size_t)n) {          /* should not happen */
        return (seq_t)0;
    }
    /* The array is unsorted; sort it. */
    qsort(arr, len, sizeof(seq_t), cmp_seq);
    seq_t result = arr[n - 1];
    free(arr);
    return result;
#endif
}
