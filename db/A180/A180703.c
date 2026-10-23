/*
 * A180703  Smallest power of 18 that begins with n.
 * https://oeis.org/A180703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest power of 18 whose decimal expansion begins with
 *            the decimal digits of n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental construction of powers of 18 and checks the leading digits. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Global cache of computed powers of 18.  powers[0] = 18^0 = 1. */
static seq_t *powers = NULL;
static size_t powers_len = 0;
static size_t powers_cap = 0;

/* Ensure that powers[0 .. k] are available. */
static void ensure_powers_up_to(idx_t k) {
    if (k < powers_len) return;
    if (powers_cap == 0) {
        powers_cap = 1;
        powers = (seq_t *)malloc(sizeof(seq_t) * powers_cap);
        powers[0] = (seq_t)1;   /* 18^0 = 1 */
        powers_len = 1;
    }
    while (powers_len <= (size_t)k) {
        /* Resize if necessary. */
        if (powers_len == powers_cap) {
            size_t new_cap = powers_cap * 2;
            seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * new_cap);
            for (size_t i = 0; i < powers_len; ++i) new_arr[i] = powers[i];
            free(powers);
            powers = new_arr;
            powers_cap = new_cap;
        }
        /* Compute next power. */
        seq_t prev = powers[powers_len - 1];
        seq_t next = seq_mul(prev, (seq_t)18);
        powers[powers_len++] = next;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* n >= 1 by definition. */
    if (n == 1) {          /* 18^0 = 1 */
        ensure_powers_up_to(0);
        return powers[0];
    }

    int digits_n = digit_len((seq_t)n);

    /* Start checking from exponent 0 upwards. */
    for (idx_t k = 0;; ++k) {
        ensure_powers_up_to(k);
        seq_t val = powers[k];
        int digits_k = digit_len(val);

        if (digits_k < digits_n) continue;   /* Too short to have n as prefix. */

        int delta = digits_k - digits_n;
        seq_t divisor = seq_pow((seq_t)10, (idx_t)delta);
        seq_t leading = val / divisor;
        if (leading == (seq_t)n) return val;
        /* Loop continues until match is found. */
    }
}
