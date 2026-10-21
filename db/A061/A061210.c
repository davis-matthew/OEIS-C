/*
 * A061210  Numbers which are the fourth powers of their digit sum.
 * https://oeis.org/A061210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061210 – numbers equal to (digit sum)^4; computed by iterating
   s, computing s^4, and checking that the digit sum equals s. */

/* Include standard for dynamic allocation */
#include <stdlib.h>

/* Compute digit sum of a seq_t value. */
static seq_t digit_sum_seq_t(seq_t x)
{
    seq_t sum = 0;
    if (x == 0) return 0;
    while (x > 0)
    {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Return the nth element (offset 1) of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;

    /* First call: generate all values.  s^4 is monotone in s,
       and the largest known solution is 36^4, so a small bound suffices. */
    if (cap == 0)
    {
        /* Allocate a modest initial capacity. */
        cap = 16;
        vals = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!vals) return 0;

        for (idx_t s = 0; s <= 1000; ++s)      /* 1000 is generous */
        {
            seq_t val = (seq_t)s * s * s * s;   /* compute s^4 */
            if (digit_sum_seq_t(val) == (seq_t)s)
            {
                if (len == cap)            /* grow array when needed */
                {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(vals, sizeof(seq_t) * cap);
                    if (!tmp) { free(vals); return 0; }
                    vals = tmp;
                }
                vals[len++] = val;
            }
        }
    }

    if (n == 0 || n > len) return 0;  /* sequence ends after len elements */
    return vals[n - 1];
}
