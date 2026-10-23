/*
 * A045732  Fibonacci numbers having initial digit '8'.
 * https://oeis.org/A045732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   digit_len, grow_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth Fibonacci number whose initial decimal digit is 8. */

#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static state for caching computed terms */
    static seq_t *terms = NULL;   /* array of found Fibonacci numbers */
    static size_t terms_cap = 0;  /* capacity of the array */
    static size_t terms_cnt = 0;  /* number of terms stored */

    /* state for generating Fibonacci numbers in order */
    static seq_t f_prev = 0;      /* F(k-1) */
    static seq_t f_curr = 1;      /* F(k)   */

    /* offset starts at 1; n == 0 is undefined but return 0 for safety */
    if (n == 0)
        return 0;

    /* Generate Fibonacci numbers until we have n terms of the sequence. */
    while (terms_cnt < n)
    {
        /* Check if the current Fibonacci number starts with digit '8'. */
        int len   = digit_len(f_curr);                      /* number of decimal digits */
        seq_t pow = pow10((idx_t)len - 1);                  /* 10^(len-1) */
        seq_t lead = f_curr / pow;                          /* leading decimal digit */

        if (lead == (seq_t)8)
        {
            /* Grow the cache if necessary and store the term. */
            if (terms_cnt >= terms_cap)
                grow_seq(&terms, &terms_cap, terms_cnt + 1);
            terms[terms_cnt++] = f_curr;
        }

        /* Advance to the next Fibonacci number. */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Return the n-th stored term (1‑based). */
    return terms[n - 1];
}
