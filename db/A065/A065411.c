/*
 * A065411  Fibonacci numbers whose digits sum to a square.
 * https://oeis.org/A065411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   digit_sum_seq, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065411: Fibonacci numbers whose digit sum is a perfect square (OEIS) */
/* The routine iterates over Fibonacci numbers, checks whether the sum of
   their decimal digits is a perfect square, stores those values in a
   static array and returns the n‑th element (1‑based).  Duplicate
   Fibonacci values are skipped.  For fixed‑width integer builds the
   routine stops once the next Fibonacci number would overflow. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum_seq.h"
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* ---------- memoised sequence storage ---------- */
    static seq_t *terms = NULL;      /* array of found terms      */
    static size_t terms_len = 0;     /* number of terms stored    */
    static size_t terms_cap = 0;     /* allocated capacity        */

    /* ---------- Fibonacci state ---------- */
    static seq_t f_prev = 0;         /* current Fibonacci number  */
    static seq_t f_curr = 1;         /* next Fibonacci number     */
    static bool init = false;        /* one‑time initialisation   */

    if (!init) {
        f_prev = 0;
        f_curr = 1;
        init = true;
    }

    /* ----------- generate terms until we have n of them ----------- */
    while (terms_len < n) {
        seq_t cur = f_prev;                     /* current Fibonacci value */

        /* test digit sum for perfect square */
        seq_t ds = digit_sum_seq(cur);
        seq_t r  = sqrt_floor(ds);
        if (r * r == ds) {
            /* avoid duplicate Fibonacci values (e.g. the second “1”) */
            if (terms_len == 0 || cur != terms[terms_len - 1]) {
                /* grow array if necessary */
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = cur;
            }
        }

        /* advance Fibonacci sequence */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;

        /* stop if further Fibonacci numbers would overflow
           (only relevant for fixed‑width integer builds) */
        if (f_prev > f_curr) break;
    }

    /* the requested term must exist in the generated list */
    return (n <= terms_len) ? terms[n - 1] : 0;
}
