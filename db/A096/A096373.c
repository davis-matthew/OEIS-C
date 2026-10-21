/*
 * A096373  Number of partitions of n such that the least part occurs exactly
 *          twice.
 * https://oeis.org/A096373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 151, u64 n <= 465,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096373: Number of partitions of n such that the least part occurs exactly twice
   Formula: a(n) = 2*p(n+2) - p(n+3) - p(n), where p(k) is the partition function. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <algorithm>
#endif

seq_t a(idx_t n) {
    /* static cache of partition numbers p[0..max] */
    static seq_t *part = NULL;
    static size_t part_cap = 0;
    static size_t part_max = 0;   /* highest index already computed */

    /* we need p up to n+3 */
    idx_t required = n + 3;
    size_t needed_cap = (size_t)required + 1;   /* indices 0 .. required */

    /* allocate or resize cache if necessary */
    if (part_cap < needed_cap) {
        size_t new_cap = needed_cap;
#ifdef __cplusplus
        seq_t *new_arr = new seq_t[new_cap];
        if (part) {
            std::copy(part, part + part_cap, new_arr);
            delete[] part;
        } else {
            new_arr[0] = seq_t(1);   /* p[0] = 1 */
        }
        part = new_arr;
#else
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (part) {
            memcpy(new_arr, part, part_cap * sizeof(seq_t));
            free(part);
        } else {
            new_arr[0] = (seq_t)1;  /* p[0] = 1 */
        }
        part = new_arr;
#endif
        part_cap = new_cap;
        if (!part) return (seq_t)0;  /* allocation failed, return 0 */
        /* If this is the first allocation, we have set part[0] = 1 above */
        if (part_max == 0) part_max = 0;   /* p[0] is ready */
    }

    /* compute missing partition numbers up to 'required' */
    for (idx_t k = part_max + 1; k <= required; ++k) {
        seq_t plus = 0, minus = 0;
        for (idx_t j = 1; ; ++j) {
            idx_t pent1 = j * (3 * j - 1) / 2;
            if (pent1 > k) break;
            if (j & 1) plus += part[k - pent1];
            else minus += part[k - pent1];

            idx_t pent2 = j * (3 * j + 1) / 2;
            if (pent2 > k) continue;
            if (j & 1) plus += part[k - pent2];
            else minus += part[k - pent2];
        }
        part[k] = plus - minus;  /* result is non‑negative */
    }
    part_max = required;

    /* compute a(n) = 2*p(n+2) - p(n+3) - p(n) */
    seq_t tmp = part[n + 2];
    tmp = tmp + tmp;           /* 2 * p(n+2) */
    tmp = tmp - part[n + 3];   /* subtract p(n+3)   */
    tmp = tmp - part[n];       /* subtract p(n)     */
    return tmp;
}
