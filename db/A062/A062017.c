/*
 * A062017  Numbers with all even digits which can be factored into two
 *          numbers (> 1) one with all odd digits and the other with all even
 *          digits.
 * https://oeis.org/A062017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   all_even_digits_uint64, all_odd_digits_uint64,
 *            has_valid_factorization_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062017: Numbers with all even digits which can be factored into
   two numbers (>1), one with all odd digits and the other with all even
   digits.  The function computes the n‑th term by brute‑force search,
   caching previously computed terms.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/all_even_digits_uint64.h"
#include "../utils/all_odd_digits_uint64.h"
#include "../utils/has_valid_factorization_uint64.h"

static seq_t *seq_table = NULL;    /* array of found terms          */
static idx_t  seq_size  = 0;       /* allocated capacity            */
static idx_t  seq_count = 0;       /* currently stored terms        */
static uint64_t next_scan = 0;     /* integer to test next          */

/* append a new term to the cached sequence array, resizing if necessary */
static void append_term(idx_t val)
{
    if (seq_count >= seq_size) {
        size_t new_size = (seq_size == 0) ? 16 : seq_size * 2;
        seq_t *tmp = (seq_t *)realloc(seq_table, new_size * sizeof(seq_t));
        seq_table = tmp;
        seq_size = (idx_t)new_size;
    }
    seq_table[seq_count++] = (seq_t)val;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (seq_count < n) {
        /* handle the special case of 0 as the first term */
        if (next_scan == 0) {
            append_term(0);
            /* next integer to test is 1 */
            next_scan = 1;
            continue;
        }

        /* only test integers that are composed entirely of even digits */
        if (all_even_digits_uint64(next_scan) &&
            has_valid_factorization_uint64(next_scan)) {
            append_term(next_scan);
        }

        ++next_scan;
    }
    return seq_table[n - 1];
}
