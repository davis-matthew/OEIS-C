/*
 * A066122  Numbers that in base 2 need one 'Reverse and Add' step to reach a
 *          palindrome.
 * https://oeis.org/A066122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_bin_pal, rev_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that need one "Reverse and Add" step (base 2) to reach a
   binary palindrome.  A number k is included iff
     1) k itself is not a binary palindrome, and
     2) k + reverse_bits(k) is a binary palindrome.          */
/* Implementation uses only integer arithmetic and bitwise operators, */
/* and is compliant with the requirements of idx_t and seq_t.       */

#include <stddef.h>   /* for NULL, size_t */
#include <stdlib.h>   /* for malloc, realloc */
#include <stdbool.h>  /* for bool */
#include "../utils/is_bin_pal.h"
#include "../utils/rev_bits.h"

seq_t a(idx_t n)
{
    /* static tables for on‑demand generation */
    static seq_t *terms      = NULL;
    static size_t count      = 0;
    static size_t capacity   = 0;
    static idx_t next_num    = 1;      /* natural number to test next */

    /* initializer */
    if (terms == NULL) {
        capacity = 128;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (terms == NULL) return 0;  /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (count < (size_t)n) {
        ++next_num;                    /* test the next natural number */
        seq_t k = (seq_t)next_num;

        /* skip if k is already a palindrome */
        if (is_bin_pal(k)) continue;

        seq_t r   = rev_bits(k);
        seq_t sum = k + r;

        /* accept only if sum is a palindrome (and k is not) */
        if (!is_bin_pal(sum)) continue;

        /* store the new term */
        if (count == capacity) {
            capacity *= 2;
            seq_t *tmp = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            terms = tmp;
        }
        terms[count++] = k;
    }

    return terms[n - 1];
}
