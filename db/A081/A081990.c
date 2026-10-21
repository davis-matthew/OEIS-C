/*
 * A081990  Numbers k that have no zero digits and such that both k+1 and
 *          (product of digits of k) + 1 are squares.
 * https://oeis.org/A081990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   digit_product, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081990: numbers k that have no zero digits, k+1 is a square, and
   product_of_digits(k)+1 is a square.  The function searches k = m*m-1
   for increasing m until it has found the n‑th term. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* Append a value to a dynamic array of seq_t. */
static void append_term(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        /* cast required for C++ compatibility when seq_t is a class type */
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr  = tmp;
        *cap  = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Return the n‑th term of the sequence.  The index is 1‑based. */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t m_next    = 2;              /* start with m = 2  (k = 3) */

    if (n == 0) return 0;                       /* undefined, guard clause */

    /* Generate terms until we have the n‑th one. */
    while (terms_len < (size_t)n) {
        seq_t m = (seq_t)m_next;
        seq_t k = m * m - (seq_t)1;             /* k = m^2 - 1 */

        seq_t prod = digit_product(k);          /* product of digits of k */
        if (prod != 0) {                        /* no zero digit */
            seq_t p_plus = prod + (seq_t)1;
            seq_t s = floor_sqrt(p_plus);
            if (s * s == p_plus) {              /* (product + 1) is a square */
                append_term(&terms, &terms_len, &terms_cap, k);
            }
        }

        m_next++;                               /* test next m */
    }

    return terms[(size_t)(n - 1)];
}
