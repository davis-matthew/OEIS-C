/*
 * A022941  a(n) = a(n-1) + c(n-2) for n >= 3, a( ) increasing, given a(1)=1,
 *          a(2)=2; where c( ) is complement of a( ).
 * https://oeis.org/A022941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; a(n)=a(n-1)+c(n-2) for n>=3, where c(n) is the
 *            complement (positive integers not in the sequence)
 * Range:     u8 n <= 20, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *a_arr = NULL;   /* sequence a */
    static seq_t *c_arr = NULL;   /* complement c */
    static size_t a_cap = 0, c_cap = 0;
    static size_t a_len = 0, c_len = 0;
    static seq_t next_candidate = 1;   /* next integer to test for c */
    static size_t comp_a_index = 0;    /* index into a_arr for complement search */

    if (n == 0) return 0;   /* out of range, return 0 (unused) */

    /* Grow the sequence until it covers the requested index */
    while (a_len < (size_t)n) {
        /* Ensure enough complement terms exist */
        while (a_len >= 2 && c_len < a_len - 1) {
            seq_t x = next_candidate;
            while (1) {
                /* Advance comp_a_index past a_arr elements smaller than x */
                while (comp_a_index < a_len && a_arr[comp_a_index] < x)
                    ++comp_a_index;

                /* If x is an a value, skip it */
                if (comp_a_index < a_len && a_arr[comp_a_index] == x) {
                    ++x;
                    continue;
                }

                /* x is missing; store it as the next c element */
                if (c_len >= c_cap) {
                    size_t new_cap = c_cap ? c_cap << 1U : 8U;
                    seq_t *tmp = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);  /* allocation failure */
                    c_arr = tmp;
                    c_cap = new_cap;
                }
                c_arr[c_len++] = x;
                next_candidate = x + 1;
                break;
            }
        }

        /* Compute the next a term */
        seq_t new_val;
        if (a_len == 0) {
            new_val = 1;           /* a(1) */
        } else if (a_len == 1) {
            new_val = 2;           /* a(2) */
        } else {
            new_val = a_arr[a_len - 1] + c_arr[a_len - 2];
        }

        /* Grow the a array if needed */
        if (a_len >= a_cap) {
            size_t new_cap = a_cap ? a_cap << 1U : 8U;
            seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);  /* allocation failure */
            a_arr = tmp;
            a_cap = new_cap;
        }
        a_arr[a_len++] = new_val;
    }

    return a_arr[n - 1];
}
