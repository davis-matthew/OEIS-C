/*
 * A217445  Numbers n such that n! has the same number of terminating zeros in
 *          bases 3 and 4.
 * https://oeis.org/A217445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_of_2, exp_of_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers n such that the number of trailing zeros of n! in base 3
   equals that in base 4. The condition is: v_3(n!) == floor(v_2(n!)/2).
   Here v_p(n!) is the exponent of prime p in n! (Legendre's formula). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/exp_of_2.h"
#include "../utils/exp_of_3.h"

/* Global state for memoized sequence */
static seq_t *seq          = NULL;   /* array of terms */
static size_t seq_cap      = 0;      /* capacity of seq */
static size_t seq_len      = 0;      /* number of terms stored */
static idx_t  last_checked = 0;      /* last integer n that was examined */
static unsigned long long acc_v2 = 0;/* cumulative exponent of 2 in n! */
static unsigned long long acc_v3 = 0;/* cumulative exponent of 3 in n! */

/* ensure seq has capacity at least min_cap */
static void ensure_capacity(size_t min_cap) {
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* generate terms until we have at least needed terms */
static void generate_terms(size_t needed) {
    while (seq_len < needed) {
        last_checked++; /* next integer to examine */
        acc_v2 += exp_of_2(last_checked);
        acc_v3 += exp_of_3(last_checked);
        if (acc_v3 == (acc_v2 >> 1)) { /* floor(v2/2) */
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
}

/* public function: returns the nth term of the sequence (1‑based index) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for index 0 – return 0 */
    if ((size_t)n > seq_len) {
        generate_terms((size_t)n);
    }
    return seq[(size_t)n - 1];
}
