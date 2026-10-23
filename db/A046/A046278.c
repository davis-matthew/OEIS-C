/*
 * A046278  a(n) is the concatenation of the first n lucky numbers.
 * https://oeis.org/A046278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of the first n lucky numbers
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t */
#include <stddef.h>   /* size_t */
#include <math.h>     /* log */
#include "../utils/digit_len.h"  /* int digit_len(seq_t) */

static idx_t *lucky   = NULL;   /* cached lucky numbers */
static size_t lucky_len = 0;    /* number of lucky numbers stored */

/* ------------------------------------------------------------------ */
/* Generate at least `need` lucky numbers and store them in `lucky`.   */
/* The function is idempotent – successive calls only enlarge the    */
/* cache when necessary.                                               */
/* ------------------------------------------------------------------ */
static void generate_lucky_upto(size_t need)
{
    if (lucky_len >= need) return;          /* cache already large enough */

    /* The natural algorithm generates lucky numbers by sieving an odd
       list.  A simple upper bound is used as an initial limit; if the
       generated list is too short we repeat with a larger limit.   */
    while (1) {
        /* Rough upper bound for the `need`‑th lucky number (good
           enough for `need` up to many millions).                              */
        size_t limit = (size_t)(need * (log((double)need) + 1.0) * 4.0 + 20.0);
        if (limit < 20) limit = 20;          /* safety for very small `need` */

        /* `arr` will hold the odd numbers ≤ limit, which are
           progressively sieved.  Allocate it once and keep it in
           a buffer.                                                    */
        idx_t *arr = (idx_t *)malloc(((limit + 1) / 2) * sizeof(idx_t));
        if (!arr) return;                    /* allocation failure → abort */

        /* Initialise arr with 1,3,5,...,limit.                           */
        size_t len = 0;
        for (idx_t x = 1; x <= (idx_t)limit; x += 2) {
            arr[len++] = x;
        }

        /* Classic lucky sieve.                                            */
        for (size_t idx = 1; idx < len; ++idx) {
            idx_t step = arr[idx];
            size_t write = 0;
            for (size_t i = 0; i < len; ++i) {
                if (((i + 1) % step) != 0) {
                    arr[write++] = arr[i];
                }
            }
            len = write;
        }

        /* If we now have enough lucky numbers, store them and exit.   */
        if (len >= need) {
            lucky = (idx_t *)realloc(lucky, len * sizeof(idx_t));
            if (lucky) {
                for (size_t i = 0; i < len; ++i) lucky[i] = arr[i];
                lucky_len = len;
            }
            free(arr);
            return;
        }

        /* Otherwise we failed; free the buffer and try again with a
           larger `limit`.  (The loop will finish almost immediately in
           practice.)                                               */
        free(arr);
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* n < 1 is outside the offset */

    generate_lucky_upto((size_t)n);          /* ensure we have at least n numbers */

    seq_t result = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        seq_t val   = (seq_t)lucky[i];
        int   digs  = digit_len(val);        /* number of decimal digits */

        seq_t pow10 = 1;
        for (int j = 0; j < digs; ++j) pow10 *= (seq_t)10;

        result = result * pow10 + val;       /* concatenate */
    }
    return result;
}
