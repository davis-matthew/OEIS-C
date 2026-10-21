/*
 * A061744  a(n) = (2^A000959(n)) - 1.
 * https://oeis.org/A061744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^A000959(n) - 1 (A000959 = lucky numbers)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"

static idx_t *lucky_arr = NULL;   /* stored lucky numbers */
static size_t lucky_count = 0;    /* number of stored lucky numbers */

/* generate at least 'need' lucky numbers and store them in lucky_arr */
static void ensure_lucky(idx_t need)
{
    if (need == 0)
        return;
    if (lucky_count >= need)
        return;

    /* free old cache if it exists */
    if (lucky_arr) {
        free(lucky_arr);
        lucky_arr = NULL;
    }
    lucky_count = 0;

    /* estimate an upper bound for the value of the nth lucky number.
       3*n is generous enough for n <= 10000.  We'll double if needed. */
    size_t limit = (size_t)need * 3 + 1;
    if (limit < 3) limit = 3;

    for (;;) {
        /* number of odd numbers up to 'limit' */
        size_t cur_len = (limit + 1) / 2;
        idx_t *cur = (idx_t *)malloc(cur_len * sizeof(idx_t));
        if (!cur) { /* allocation failure – abort */
            exit(1);
        }

        /* initialise cur with odd numbers : 1,3,5,... */
        for (size_t i = 0; i < cur_len; ++i)
            cur[i] = (idx_t)(2 * i + 1);

        size_t len = cur_len;          /* current number of elements */

        /* sieve: remove every kth element where k is the kth element
           of the current list (starting from the second element). */
        for (size_t i = 1; i < len; ++i) {
            idx_t step = cur[i];
            if (step > (idx_t)len)
                break; /* no further eliminations possible */

            size_t new_len = 0;
            size_t count   = 1;
            for (size_t j = 0; j < len; ++j) {
                if (count % step != 0)
                    cur[new_len++] = cur[j];
                ++count;
            }
            len = new_len;
        }

        if (len >= need) {
            lucky_arr = (idx_t *)malloc(need * sizeof(idx_t));
            if (!lucky_arr) { free(cur); exit(1); }
            for (size_t i = 0; i < need; ++i)
                lucky_arr[i] = cur[i];
            lucky_count = need;
            free(cur);
            break;
        }

        free(cur);
        /* not enough numbers – increase the limit and retry */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – undefined for n=0 */
        return 0;

    ensure_lucky(n);

    idx_t exp = lucky_arr[n - 1];           /* nth lucky number */
    seq_t val = pow2(exp);                 /* 2^exp */
    return val - (seq_t)1;                 /* 2^exp - 1 */
}
