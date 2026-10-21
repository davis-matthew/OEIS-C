/*
 * A214853  Fibonacci numbers with only one 0 in the binary representation.
 * https://oeis.org/A214853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is the n-th Fibonacci number that has exactly one 0 * in
 *            its binary representation (0 itself counts as having one 0)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   one_zero_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence on demand and memoises
 * the discovered terms.  It uses the provided fib_pair routine to
 * generate successive Fibonacci numbers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/one_zero_binary.h"

/* Forward declaration of the fib function provided by the kernel */
extern seq_t fib(idx_t n);

/* Cached sequence data */
static seq_t *terms      = NULL;   /* array of discovered terms */
static size_t terms_cap  = 0;      /* allocated capacity of terms   */
static size_t terms_len  = 0;      /* actual number of terms found  */

/* State for on-demand Fibonacci generation */
static idx_t  fib_idx    = 0;           /* current Fibonacci index */
static seq_t  fib_prev   = 0;           /* fib(fib_idx)          */
static seq_t  fib_cur    = 1;           /* fib(fib_idx+1)        */

/* Append a value to the cached array, growing if necessary */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        /* double capacity on growth */
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* insufficient memory */
        terms      = tmp;
        terms_cap  = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n==0, but guard */

    /* Build terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        if (one_zero_binary(fib_prev))
            append_term(fib_prev);

        /* Advance Fibonacci */
        seq_t f_next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur  = f_next;
        ++fib_idx;
    }
    return terms[n - 1];
}
