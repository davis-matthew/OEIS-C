/*
 * A045727  Fibonacci numbers having initial digit '3'.
 * https://oeis.org/A045727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th Fibonacci number whose first decimal digit is 3.
 *  The function searches Fibonacci indices in increasing order, keeps a
 *  static cache of the discovered terms, and returns a(n) in constant
 *  time for repeated queries.  Leading digits are obtained by dividing
 *  by the appropriate power of ten.  No floating‑point arithmetic is
 *  used, so the implementation works for any seq_t type (8, 16, 32,
 *  64 or 128 bit or arbitrary precision).
 */

#include <stdlib.h>    /* malloc, realloc */
#include <stddef.h>    /* size_t */
#include <stdbool.h>   /* bool */

#include "../utils/digit_len.h"   /* digit_len() */
#include "../utils/pow10.h"       /* pow10() */

seq_t a(idx_t n)
{
    /*--- Static cache of discovered Fibonacci numbers ------------------*/
    static seq_t *cache = NULL;   /* array of values  */
    static size_t cap   = 0;      /* current capacity   */
    static size_t used  = 0;      /* number of terms stored */

    /*--- Variables controlling the Fibonacci scan ---------------------*/
    static idx_t  k       = 1;    /* current Fibonacci index to examine   */
    static seq_t prev_fib = 0;    /* F(k-1)                               */
    static seq_t curr_fib = 1;    /* F(k)                                 */
    static size_t last_digits = (size_t)-1;  /* cache for pow10 value    */
    static seq_t pow10_val = 0;               /* 10^(digits-1) for curr_fib  */

    /*--- Allocate initial cache buffer. --------------------------------*/
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /*--- Generate terms until we have at least n of them. ----------------*/
    while (used < n) {
        /* Estimate number of digits in curr_fib.                    */
        size_t d = digit_len(curr_fib);

        /* Update the power of ten divisor when the digit count changes. */
        if (d != last_digits) {
            pow10_val  = pow10((idx_t)(d - 1));
            last_digits = d;
        }

        /* Extract the most significant decimal digit.                */
        seq_t first_digit = curr_fib / pow10_val;
        if (first_digit == (seq_t)3) {
            /* Store the discovered Fibonacci number.                  */
            if (used >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[used++] = curr_fib;
        }

        /* Advance to the next Fibonacci number. */
        seq_t next_fib = prev_fib + curr_fib;
        prev_fib = curr_fib;
        curr_fib = next_fib;
        ++k;
    }

    return cache[n - 1];
}
