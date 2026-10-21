/*
 * A225166  Denominators of the sequence s(n) of the sum resp. product of
 *          fractions f(n) defined recursively by f(1) = 7/1; f(n+1) is chosen
 *          so that the sum and the product of the first n terms of the
 *          sequence are equal.
 * https://oeis.org/A225166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = a(n-1) * (7^(2^(n-2)) - a(n-1)) for n>1, a(1)=1 */
/* The recurrence follows from a(n)=7^(2^(n-1))*b(n) with b(n)=b(n-1)-b(n-1)^2.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: memo[i] holds a(i) for 1 <= i <= current_len.
       pow7 holds 7^(2^(current_len-1)), the factor used for the next step */
    static seq_t *memo = NULL;
    static idx_t current_len = 0;
    static seq_t pow7 = 0;

    /* initialisation */
    if (!memo)
    {
        memo = (seq_t*)calloc(2, sizeof(seq_t)); /* indices 0..1, index 0 unused */
        if (!memo) return (seq_t)0;              /* out of memory guard */
        memo[1] = (seq_t)1;
        current_len = 1;
        pow7 = (seq_t)7;                         /* 7^(2^(1-1)) = 7^1 = 7 */
    }

    /* extend cache if needed */
    if (n > current_len)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* out of memory guard */
        memo = tmp;
        /* compute new elements */
        for (idx_t k = current_len; k < n; ++k)
        {
            /* a(k+1) = a(k) * (pow7 - a(k)) */
            seq_t diff = pow7 - memo[k];
            memo[k + 1] = memo[k] * diff;
            /* update pow7 for the next step: pow7 = pow7 * pow7  (exponent doubles) */
            pow7 = pow7 * pow7;
        }
        current_len = n;
    }

    return memo[n];
}
