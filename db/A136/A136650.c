/*
 * A136650  Self-convolution of A006125(n) = 2^{n(n-1)/2}.
 * https://oeis.org/A136650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^{k(k-1)/2 + (n-k)(n-k-1)/2} * =
 *            sum_{k=0..n} b[k] * b[n-k], where * b[k] = 2^{k(k-1)/2}
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* static storage for sequence values */
static seq_t *b_arr = NULL;   /* b[n] = 2^{n(n-1)/2}   */
static seq_t *a_arr = NULL;   /* a[n] = required values */
static idx_t   max_n = (idx_t)-1;   /* highest index computed so far */

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* expand storage if necessary */
    if (max_n == (idx_t)-1 || n > max_n) {
        idx_t start = (max_n == (idx_t)-1) ? 0 : (max_n + 1);

        /* reallocate arrays to hold indices up to n */
        seq_t *b_new = mem_resize(seq_t, b_arr, (n + 1));
        seq_t *a_new = mem_resize(seq_t, a_arr, (n + 1));
        b_arr = b_new;
        a_arr = a_new;

        for (idx_t i = start; i <= n; ++i) {
            if (i == 0) {                /* base case */
                b_arr[0] = (seq_t)1;
                a_arr[0] = (seq_t)1;
            } else {
                /* b[i] = b[i-1] * 2^{i-1} */
                seq_t mult = pow2(i - 1);          /* 2^{i-1} as seq_t */
                b_arr[i]   = seq_mul(b_arr[i - 1], mult);

                /* a[i] = sum_{k=0..i} b[k] * b[i-k] */
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k <= i; ++k) {
                    seq_t prod = seq_mul(b_arr[k], b_arr[i - k]);
                    sum += prod;
                }
                a_arr[i] = sum;
            }
        }
        max_n = n;
    }
    return a_arr[n];
}
