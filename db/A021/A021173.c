/*
 * A021173  Decimal expansion of 1/169.
 * https://oeis.org/A021173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/169) - 10*floor(10^n/169)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th decimal digit of 1/169 using
   incremental long‑division and a static cache. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static idx_t cap = 0;
    static seq_t rem = 1;  /* initial remainder = 1 */

    /* Extend cache up to index n */
    while (cap <= n)
    {
        /* Compute next digit via long division */
        seq_t q = (rem * 10) / (seq_t)169;
        rem = (rem * 10) % (seq_t)169;

        /* Resize cache */
        seq_t *tmp = (seq_t *)realloc(digits, (size_t)(cap + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 as fallback */
            return (seq_t)0;
        }
        digits = tmp;
        digits[cap] = q;
        cap++;
    }

    return digits[n];
}
