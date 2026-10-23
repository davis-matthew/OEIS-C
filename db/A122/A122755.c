/*
 * A122755  Let f(m) = 10 - last digit of prime(m). Sequence gives numbers n
 *          such that (1/n)*Sum_{ m <= n } f(m) is <= 5.
 * https://oeis.org/A122755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A122755: n such that (1/n)*∑_{m≤n}f(m) ≤ 5, 
   where f(m) = 10 – (last digit of the m‑th prime).
   The sequence is produced by scanning successive integers n,
   accumulating the sum of f values, and recording those n
   that satisfy the inequality. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Static state retained between calls */
    static idx_t computed_upto = 0;         /* last n processed          */
    static uint64_t sum_f = 0;              /* Accumulated f values      */
    static seq_t *qual = NULL;              /* Array of qualifying N's   */
    static size_t qual_cap = 0;             /* Capacity of qual array    */
    static size_t qual_len = 0;             /* Number of entries stored  */

    /* Lazily allocate qualifier array */
    if (qual_cap == 0) {
        qual_cap = 16;
        qual = (seq_t*)malloc(qual_cap * sizeof(seq_t));
    }
    /* Grow array if necessary */
    if (qual_len + 1 > qual_cap) {
        size_t new_cap = qual_cap << 1;
        if (new_cap < qual_len + 1) new_cap = qual_len + 1;
        qual = (seq_t*)realloc(qual, new_cap * sizeof(seq_t));
        qual_cap = new_cap;
    }

    /* Extend computation until we have at least n qualifying terms */
    while (qual_len < n) {
        computed_upto += 1;
        uint32_t p = get_prime(computed_upto);   /* p is the computed_upto‑th prime */
        uint32_t d = p % 10;                     /* last decimal digit               */
        uint64_t f = 10ull - (uint64_t)d;        /* f(m) value                       */
        sum_f += f;

        /* Check whether the average of f up to computed_upto is ≤ 5   */
        if (sum_f <= 5ull * (uint64_t)computed_upto) {
            qual[qual_len++] = (seq_t)computed_upto; /* store qualifying N */
        }
    }

    return qual[n - 1];
}
