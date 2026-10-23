/*
 * A104381  Numbers k such that 10^(k-1) == 1 (mod k).
 * https://oeis.org/A104381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 6616, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 10^(k-1) ≡ 1 (mod k) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_good.h"

/* Compute the nth term of A104381. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static uint64_t next_k = 1;   /* next k to test */

    /* Ensure we have at least n terms stored. */
    if (terms_size < (size_t)n) {
        /* Grow storage if needed. */
        if (terms_cap == 0) {
            terms_cap = 1024;
            terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        }
        while (terms_cap < (size_t)n) {
            terms_cap *= 2;
            terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        }
        /* Generate terms until we have n of them. */
        while (terms_size < (size_t)n) {
            if (is_good(next_k))
                terms[terms_size++] = (seq_t)next_k;
            ++next_k;
        }
    }
    return terms[n - 1];
}
