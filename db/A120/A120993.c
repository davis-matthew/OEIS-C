/*
 * A120993  a(n) is the integer in the n-th run of squarefree integers which
 *          is divisible by the number of integers in that run.
 * https://oeis.org/A120993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) – integer in the n-th run of squarefree integers divisible
 *            by the run length
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    static idx_t computed = 0;
    static idx_t cap = 0;
    static idx_t *start_arr = NULL;
    static idx_t *len_arr = NULL;
    static seq_t *value_arr = NULL;
    static idx_t cur_tested = 0;

    if (n == 0) return 0;                  /* offset is 1, but guard against 0 */
    idx_t target = n - 1;                  /* 0-based index */

    if (cap == 0) {                        /* initial allocation */
        cap = 16;
        start_arr = (idx_t *)malloc(cap * sizeof(idx_t));
        len_arr   = (idx_t *)malloc(cap * sizeof(idx_t));
        value_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (computed <= target) {
        /* Find the start of the next run of squarefree integers */
        idx_t cur = cur_tested;
        do { cur++; } while (!is_squarefree((seq_t)cur));
        idx_t start = cur;

        /* Extend the run while the numbers are squarefree */
        idx_t len = 1;
        cur++;
        while (is_squarefree((seq_t)cur)) {
            len++;
            cur++;
        }

        /* Find the element in the run divisible by its length */
        idx_t mod = start % len;
        idx_t offset = (len - mod) % len;
        seq_t a_val = (seq_t)(start + offset);

        /* Store the results */
        if (computed >= cap) {
            idx_t newcap = cap * 2;
            start_arr = (idx_t *)realloc(start_arr, newcap * sizeof(idx_t));
            len_arr   = (idx_t *)realloc(len_arr,   newcap * sizeof(idx_t));
            value_arr = (seq_t *)realloc(value_arr, newcap * sizeof(seq_t));
            cap = newcap;
        }
        start_arr[computed] = start;
        len_arr[computed]   = len;
        value_arr[computed] = a_val;

        computed++;
        cur_tested = cur;  /* first non‐squarefree number after the run */
    }

    return value_arr[target];
}
