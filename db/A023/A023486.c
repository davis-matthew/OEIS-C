/*
 * A023486  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 2) and
 *          d(n) = (n-th non-Fibonacci number).
 * https://oeis.org/A023486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fib(n+4) + non_fib(n)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A001690(n+1) + A000045(n+4) (OEIS A023486) */

#include <stdlib.h>           /* malloc, realloc */
#include <stdint.h>           /* uint64_t */
#include "../utils/fib.h"     /* external Fibonacci routine */

/* Cache for non‑Fibonacci numbers (0‑based index). */
static seq_t   *nonfib_vals = NULL;   /* array of non‑Fibonacci numbers */
static size_t  nonfib_len  = 0;       /* number of stored entries      */
static size_t  nonfib_cap  = 0;       /* allocated capacity            */

/* Current integer to test for being Fibonacci or not. */
static seq_t   curr_int    = 0;       /* next integer to examine       */

/* Current two Fibonacci numbers: f_prev ≤ curr_int < f_curr. */
static seq_t   f_prev      = 0;       /* largest Fibonacci ≤ curr_int  */
static seq_t   f_curr      = 1;       /* next Fibonacci after f_prev   */

/* Initialise the non‑Fibonacci cache if needed. */
static void init_nonfib_cache(void)
{
    if (nonfib_vals != NULL) return;      /* already initialised */
    nonfib_cap = 1024;
    nonfib_vals = (seq_t *)malloc(nonfib_cap * sizeof(seq_t));
}

/* Ensure that nonfib_vals[0 … n] are filled. */
static void ensure_nonfib_size(idx_t n)
{
    init_nonfib_cache();

    while (nonfib_len <= (size_t)n)
    {
        /* Advance Fibonacci numbers until f_curr > curr_int. */
        while (f_curr <= curr_int) {
            seq_t tmp = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = tmp;
        }
        /* curr_int is between f_prev (≤ curr_int) and f_curr (> curr_int). */
        if (curr_int != f_prev) {                /* curr_int is NOT Fibonacci */
            if (nonfib_len == nonfib_cap) {       /* grow the array */
                nonfib_cap <<= 1;
                nonfib_vals = (seq_t *)realloc(nonfib_vals,
                                                nonfib_cap * sizeof(seq_t));
            }
            nonfib_vals[nonfib_len++] = curr_int; /* cache this non‑Fib number */
        }
        curr_int++;                              /* move to the next integer  */
    }
}

/* Public sequence function: a(n) */
seq_t a(idx_t n)
{
    ensure_nonfib_size(n);

    seq_t fib_val = fib(n + 4);          /* (n+4)-th Fibonacci number (>2) */
    seq_t d_val   = nonfib_vals[(size_t)n]; /* n‑th non‑Fibonacci number       */

    return fib_val + d_val;
}
