/*
 * A037372  Positive numbers k such that every base-2 digit of k is a base-3
 *          digit of k.
 * https://oeis.org/A037372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9909 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive numbers k such that every base-2 digit of k is a base-3 digit of k */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the ternary representation of x contains the digit d. */
static inline bool has_ternary_digit(seq_t x, int d)
{
    if (x == 0)
        return d == 0;               /* 0 in ternary is "0" */
    while (x > 0) {
        int digit = (int)(x % 3);
        if (digit == d)
            return true;
        x /= 3;
    }
    return false;
}

/* Determine whether k satisfies the sequence condition. */
static inline bool ternary_has_for_all(seq_t k)
{
    /* Digit 1 must always be present. */
    if (!has_ternary_digit(k, 1))
        return false;

    /* If k is not of the form 2^m‑1, its binary representation
       contains at least one zero bit; then digit 0 must appear in
       ternary representation. */
    seq_t kp1 = k + 1;
    if ((kp1 & (kp1 - 1)) != 0) {   /* kp1 not a power of two */
        if (!has_ternary_digit(k, 0))
            return false;
    }
    return true;
}

static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_len  = 0;
static seq_t  next_k     = 1;

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – undefined below */
        return 0;

    /* Produce terms up to the required index. */
    while (terms_len < (size_t)n) {
        if (ternary_has_for_all(next_k)) {
            /* Ensure enough capacity and store the term. */
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp)
                    exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_k;
        }
        ++next_k;
    }

    return terms[(size_t)n - 1];
}
