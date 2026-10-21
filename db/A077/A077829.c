/*
 * A077829  Expansion of 1/(1-3*x-3*x^2-2*x^3).
 * https://oeis.org/A077829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 2*a(n-3), with initial values
 *            a(0)=1, a(1)=3, a(2)=12
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0, len = 0;

    /* Return cached value if already computed */
    if (n < (idx_t)len)
        return seq[(size_t)n];

    /* Ensure capacity for n+1 elements */
    size_t target = (size_t)n;
    if (target >= cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= target) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute values from len up to n */
    while (len <= target) {
        seq_t value;
        if (len == 0)          value = 1;
        else if (len == 1)     value = 3;
        else if (len == 2)     value = 12;
        else                   value = 3 * seq[len-1] + 3 * seq[len-2] + 2 * seq[len-3];
        seq[len] = value;
        ++len;
    }

    return seq[(size_t)n];
}
