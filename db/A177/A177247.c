/*
 * A177247  Fibonacci numbers Fib(n) whose decimal expansion does not contain
 *          any digit 6.
 * https://oeis.org/A177247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Fibonacci numbers Fib(k) whose decimal expansion does not
 *            contain any digit 6
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether a seq_t value contains no decimal digit 6. */
static bool has_no_digit6(seq_t x)
{
    seq_t t = x;
    while (t != 0) {
        if ((unsigned)(t % 10) == 6) {
            return false;
        }
        t /= 10;
    }
    return true;
}

/* The a(n) function. */
seq_t a(idx_t n)
{
    /* static cache for valid Fibonacci numbers */
    static seq_t *valid = NULL;      /* array of seq_t */
    static size_t  cap   = 0;        /* current capacity */
    static size_t  len   = 0;        /* number of stored values */

    /* static state for Fibonacci generation */
    static seq_t f_prev = 1;  /* F(1) */
    static seq_t f_curr = 1;  /* F(2) */
    static idx_t  fib_index = 2; /* next Fibonacci index to compute (starting after F(2)) */
    static bool initialized = false;

    /* initialize cache on first call */
    if (!initialized) {
        cap = 16;
        valid = mem_new(seq_t, cap);
        /* store the first two Fibonacci numbers if they satisfy the condition */
        size_t i = 0;
        if (has_no_digit6(f_prev)) {
            valid[i++] = f_prev;
        }
        if (has_no_digit6(f_curr)) {
            valid[i++] = f_curr;
        }
        len = i;
        initialized = true;
    }

    /* produce more Fibonacci numbers until we have at least n valid ones */
    while (len < n) {
        seq_t next = f_prev + f_curr;
        f_prev   = f_curr;
        f_curr   = next;
        fib_index++;

        if (has_no_digit6(next)) {
            if (len == cap) {
                /* grow the cache */
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, valid, newcap);
                if (!tmp) {
                    /* allocation failed; return 0 as a fallback */
                    return (seq_t)0;
                }
                valid = tmp;
                cap = newcap;
            }
            valid[len++] = next;
        }
    }

    return valid[n - 1];
}
