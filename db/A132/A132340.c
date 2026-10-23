/*
 * A132340  a(n+1) = if {a(k):1<=k<=n} is a permutation of [1:n] then 2*a(n)
 *          else a(n)-1.
 * https://oeis.org/A132340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = if {a(k):1<=k<=n} is a permutation of [1..n] then
 *            2*a(n) else a(n)-1
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Computes OEIS sequence A132340. */
seq_t a(idx_t n)
{
    /* static cached data */
    static seq_t *arr = NULL;
    static idx_t arr_cap = 0;
    static uint16_t *freq = NULL;
    static idx_t freq_cap = 0;
    static idx_t max_seen = 0;          /* largest value seen so far */
    static idx_t computed = 0;          /* last computed index */

    /* initialise first term */
    if (computed == 0) {
        if (arr_cap < 2) {
            arr_cap = 2;
            arr = (seq_t *)calloc(arr_cap, sizeof(seq_t));
        }
        if (freq_cap < 2) {
            freq_cap = 2;
            freq = (uint16_t *)calloc(freq_cap, sizeof(uint16_t));
        }
        arr[1] = (seq_t)1;
        freq[1] = 1;
        max_seen = 1;
        computed = 1;
    }

    if (n <= computed) {
        return arr[n];
    }

    /* ensure array capacity */
    if ((size_t)n >= arr_cap) {
        size_t new_cap = (size_t)arr_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        arr = tmp;
        arr_cap = (idx_t)new_cap;
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t prev_idx = i - 1;
        seq_t prev_val = arr[prev_idx];

        /* test whether first prev_idx terms form a permutation of 1..prev_idx */
        bool is_perm = true;
        if (max_seen > prev_idx) {
            is_perm = false;
        } else {
            for (idx_t k = 1; k <= prev_idx; ++k) {
                if (freq[k] != 1) {
                    is_perm = false;
                    break;
                }
            }
        }

        seq_t new_val = is_perm ? prev_val * (seq_t)2 : prev_val - (seq_t)1;
        arr[i] = new_val;

        /* update frequency table */
        idx_t val_idx = (idx_t)new_val;
        if (val_idx + 1 > freq_cap) {
            size_t old_cap = (size_t)freq_cap;
            size_t new_cap = old_cap ? old_cap : 1;
            while (new_cap <= (size_t)val_idx) new_cap <<= 1;
            uint16_t *tmpf = (uint16_t *)realloc(freq, new_cap * sizeof(uint16_t));
            if (!tmpf) exit(EXIT_FAILURE);
            freq = tmpf;
            memset(freq + old_cap, 0, (new_cap - old_cap) * sizeof(uint16_t));
            freq_cap = (idx_t)new_cap;
        }
        freq[val_idx]++;

        if (val_idx > max_seen) {
            max_seen = val_idx;
        }
    }

    computed = n;
    return arr[n];
}
