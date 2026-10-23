/*
 * A131093  a(1)=1, a(n) = a(n-1) + sum of odd numbers which are among the
 *          first (n-1) terms of the sequence.
 * https://oeis.org/A131093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = a(n-1) + sum of odd a(k) for 1 ≤ k < n
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t size = 0;        /* number of terms stored (a(1) is vals[0]) */
    static size_t cap = 0;         /* allocation capacity */
    static seq_t odd_sum = 0;      /* sum of odd terms up to the last stored index */

    if (n == 0)
        return 0;                  /* undefined by the OEIS definition */

    /* Ensure we have computed up to index n */
    if ((size_t)n > size)
    {
        /* First call: initialise storage and first term */
        if (size == 0)
        {
            cap = 1;
            vals = (seq_t *)malloc(cap * sizeof(seq_t));
            vals[0] = 1;           /* a(1) */
            odd_sum = 1;           /* a(1) is odd */
            size = 1;
        }

        /* Extend the sequence up to n */
        if ((size_t)n > cap)
            cap = (size_t)n;            /* grow enough for this call */
        vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));

        for (size_t i = size; i < (size_t)n; ++i)
        {
            /* a(i+1) = a(i) + odd_sum, where odd_sum is sum of odd terms among a(1..i) */
            seq_t prev = vals[i - 1];
            seq_t new_val = prev + odd_sum;
            vals[i] = new_val;

            /* Update odd_sum if the new term is odd */
            if (new_val & 1)
                odd_sum += new_val;
        }
        size = (size_t)n;
    }

    return vals[n - 1];
}
