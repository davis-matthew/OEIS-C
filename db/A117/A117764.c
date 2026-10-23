/*
 * A117764  Lucas numbers for which the sum of the digits is also a Lucas
 *          number.
 * https://oeis.org/A117764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Lucas numbers whose digit sum is also a Lucas number.
   Search: iterate through the Lucas sequence, keeping those whose digit
   sum is a Lucas number. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_sum.h"

static bool initialized = false;
static seq_t *lucas_arr   = NULL;   /* all generated Lucas numbers      */
static size_t lucas_len   = 0;
static size_t lucas_cap   = 0;

static seq_t *special_arr = NULL;   /* terms of A117764 found so far    */
static size_t special_len = 0;
static size_t special_cap = 0;

static seq_t prev2 = 0;   /* L_{k-2} */
static seq_t prev1 = 0;   /* L_{k-1} */

/* Resize an array of seq_t */
static void ensure_cap(seq_t **arr, size_t *len, size_t *cap) {
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Binary search over lucas_arr[0..lucas_len-1] for value v */
static bool is_lucas_value(seq_t v) {
    seq_t *lo  = lucas_arr;
    seq_t *hi  = lucas_arr + lucas_len;
    while (lo < hi) {
        seq_t *mid = lo + (hi - lo) / 2;
        if (*mid == v) return true;
        if (*mid < v)  lo = mid + 1;
        else           hi = mid;
    }
    return false;
}

/* Add a Lucas number to the global list and record it if its
   digit sum is also a Lucas number. */
static void add_lucas(seq_t val) {
    /* store the Lucas number */
    ensure_cap(&lucas_arr, &lucas_len, &lucas_cap);
    lucas_arr[lucas_len++] = val;

    /* digit sum of the Lucas number */
    seq_t sum = digit_sum(val);

    /* test membership of the sum */
    if (is_lucas_value(sum)) {
        ensure_cap(&special_arr, &special_len, &special_cap);
        special_arr[special_len++] = val;
    }
}

/* Public function required by the harness */
seq_t a(idx_t n) {
    if (!initialized) {
        /* initial capacities */
        lucas_cap   = 8;
        lucas_arr   = (seq_t *)malloc(lucas_cap * sizeof(seq_t));

        special_cap = 8;
        special_arr = (seq_t *)malloc(special_cap * sizeof(seq_t));

        prev2 = (seq_t)1;   /* L0 */
        prev1 = (seq_t)3;   /* L1 */

        /* add the first two Lucas numbers */
        add_lucas(prev2);   /* L0 */
        add_lucas(prev1);   /* L1 */

        initialized = true;
    }

    /* generate Lucas numbers until we have enough special terms */
    while (special_len <= (size_t)n) {
        seq_t next = prev1 + prev2;   /* L_k = L_{k-1} + L_{k-2} */
        prev2 = prev1;
        prev1 = next;
        add_lucas(next);
    }

    return special_arr[(size_t)n];
}
