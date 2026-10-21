/*
 * A202089  Numbers n such that n^2 and (n+1)^2 have same digit sum.
 * https://oeis.org/A202089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202089: numbers n for which digit_sum(n²) = digit_sum((n+1)²). */
/* Computed by searching successive integers and memoising the result. */

#include <stdlib.h>
#include <stdint.h>

/* Helper: digit sum of an unsigned 128‑bit integer. */
static inline unsigned digit_sum_uint128(__uint128_t x)
{
    unsigned sum = 0;
    while (x > 0) {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static storage for the sequence values */
    static seq_t *terms = NULL;    /* array of found n's (as seq_t) */
    static size_t terms_len = 0;   /* number of terms stored */
    static size_t terms_cap = 0;   /* capacity of the array */
    static idx_t next_candidate = 1; /* next integer to test */

    /* Lazy allocation of the terms array */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Compute digit sums using 128‑bit arithmetic to avoid overflow */
        __uint128_t sq1 = (__uint128_t)next_candidate * (__uint128_t)next_candidate;
        unsigned sum1 = digit_sum_uint128(sq1);

        idx_t nxt = next_candidate + 1;
        __uint128_t sq2 = (__uint128_t)nxt * (__uint128_t)nxt;
        unsigned sum2 = digit_sum_uint128(sq2);

        if (sum1 == sum2) {
            /* Store the candidate; cast to seq_t (it will fit for n we care about) */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
