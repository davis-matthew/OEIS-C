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
 * Helpers:   has_ternary_digit, ternary_has_for_all
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive numbers k such that every base-2 digit of k is a base-3 digit of k */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_ternary_digit.h"
#include "../utils/ternary_has_for_all.h"

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
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
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
