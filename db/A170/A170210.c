/*
 * A170210  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A170210 – Number of reduced words of length n in the Coxeter
 *            group on 9 generators with relations (S_i)^2 = (S_i S_j)^40 = I.
 *            The sequence satisfies the linear recurrence a(0) = 1 for 1 ≤ n
 *            ≤ 39: a(n) = 2 + 7 · Σ_{k=0}^{n-1} a(k) for n ≥ 40: a(n) = 7 ·
 *            Σ_{k=n-39}^{n-1} a(k) - 28 · a(n-40) The pro...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *buf          = NULL;  /* array holding a[0] … a[len-1] */
    static size_t len          = 0;     /* number of elements currently stored */
    static size_t cap          = 0;     /* capacity of buf */
    static seq_t total_sum     = 0;     /* Σ_{k=0}^{len-1} a(k) */
    static seq_t win_sum       = 0;     /* Σ_{k=max(1,len-39)}^{len-1} a(k) */
    static int  win_sum_ready  = 0;     /* 1 if win_sum corresponds to last 39 terms */

    /* Ensure that buf can store indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cap)
    {
        size_t new_cap = needed;
        if (new_cap < 40) new_cap = 40;              /* at least space for first 40 terms */
        if (buf == NULL)
            buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            buf = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* If we already have the requested value, return it immediately */
    if (n < len)
        return buf[n];

    /* --------------------------------------------------------------------
       Compute new terms from the current length up to n, updating the
       cumulative sums used in the recurrence.  */

    /* Start from the current end of the cache */
    for (size_t i = len; i <= (size_t)n; ++i)
    {
        if (i == 0)
        {
            /* a(0) = 1 */
            buf[0] = (seq_t)1;
            total_sum = (seq_t)1;          /* Σ a(k) for k<1 */
        }
        else if (i < 40)           /* 1 ≤ i ≤ 39 */
        {
            /* a(i) = 2 + 7 * Σ_{k=0}^{i-1} a(k) = 2 + 7*total_sum */
            buf[i] = (seq_t)2 + (seq_t)7 * total_sum;
            total_sum += buf[i];
        }
        else                      /* i ≥ 40 */
        {
            /* Prepare win_sum the first time we reach i == 40 */
            if (!win_sum_ready)
            {
                /* total_sum now equals Σ a(k) for k=0..39 (after adding a[39]) */
                win_sum = total_sum - buf[0];  /* sum of a[1]..a[39] */
                win_sum_ready = 1;
            }

            /* a(i) = 7 * win_sum - 28 * a(i-40) */
            buf[i] = (seq_t)7 * win_sum - (seq_t)28 * buf[i - 40];

            /* Update cumulative sums */
            total_sum += buf[i];
            win_sum     = win_sum + buf[i] - buf[i - 39];
        }
    }

    /* Update length after filling terms */
    len = (size_t)n + 1;

    return buf[n];
}
