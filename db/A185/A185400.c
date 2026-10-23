/*
 * A185400  Numbers with property that the digital sum plus the product of the
 *          digits is a power of 2.
 * https://oeis.org/A185400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 2453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant: numbers whose digit sum plus digit product is a power of two. */
/* a(n) is the n‑th natural number x (1‑based) with (digit_sum(x)+digit_product(x)) a power of 2. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_product_seq.h"

/* --- small helpers that work for all supported seq_t types -------------------------------- */

static inline seq_t digit_sum_seq(seq_t x)
{
    seq_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

static inline bool is_power_of_two_seq(seq_t x)
{
    if (x == 0)
        return false;
    /* works for unsigned integers and for the arbitrary‑precision class provided in the build */
    return (x & (x - 1)) == 0;
}

/* --- the sequence function --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    static seq_t *terms      = NULL;  /* cached sequence values */
    static size_t terms_cnt  = 0;     /* number of cached terms */
    static size_t terms_cap  = 0;     /* current capacity of terms */
    static seq_t cand = 1;            /* next candidate to test */

    /* grow cache to reach the required n */
    while (terms_cnt < n) {
        seq_t s  = digit_sum_seq(cand);
        seq_t p  = digit_product_seq(cand);
        seq_t sp = s + p;

        if (is_power_of_two_seq(sp)) {
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = cand;
        }
        cand = cand + 1;
    }

    return terms[n - 1];
}
