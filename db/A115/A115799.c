/*
 * A115799  Integers i such that 4*i XOR 7*i = 11*i.
 * https://oeis.org/A115799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer x such that (4*x XOR 7*x) == 11*x
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the n‑th term of OEIS sequence A115799 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0;
    static idx_t last_checked = 0;

    /* Ensure the first term 0 is present */
    if (len == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
        len = 1;
        last_checked = 0;
    }

    /* Generate terms until we have at least n+1 entries */
    while (len <= n)
    {
        ++last_checked;
        seq_t x = (seq_t)last_checked;

        /* 4*x */
        seq_t left = x << 2;

        /* 7*x = 4*x + 2*x + x */
        seq_t sum = left + (x << 1) + x;

        /* 4*x XOR 7*x */
        seq_t xor_val = left ^ sum;

        /* 11*x = 8*x + 2*x + x */
        seq_t target = (x << 3) + (x << 1) + x;

        if (xor_val == target)
        {
            seq_t *tmp = (seq_t *)realloc(cache, (len + 1) * sizeof(seq_t));
            if (tmp) cache = tmp; /* assume success */
            cache[len] = x;
            ++len;
        }
    }

    return cache[n];
}
