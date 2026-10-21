/*
 * A231667  a(n) = Sum_{i=0..n} digsum_4(i)^4, where digsum_4(i) = A053737(i).
 * https://oeis.org/A231667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_4(i)^4, where digsum_4(i) = sum of
 *            base‑4 digits of i
 * Range:     u8 n <= 6, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of cumulative sums */
    static seq_t *cum   = NULL;   /* array holding a(i) for i = 0..max_n */
    static size_t   capacity = 0; /* allocated size of cum array      */
    static idx_t    max_n    = 0; /* highest n that has been computed */

    /* Allocate cache on first call or when needed */
    if (!cum)
    {
        capacity = (n == 0) ? 1 : (size_t)(n + 1);
        cum = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cum) return 0;  /* malloc failed */
        cum[0] = 0;          /* a(0) = 0 */
        max_n   = 0;
    }

    /* Grow the cache if necessary */
    if (n > max_n)
    {
        if (capacity <= (size_t)n)
        {
            size_t new_cap = (capacity) ? capacity * 2 : (size_t)(n + 1);
            if (new_cap <= (size_t)n) new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* realloc failed */
            cum = tmp;
            capacity = new_cap;
        }

        /* Compute new terms incrementally */
        seq_t acc = cum[max_n];   /* cumulative sum up to max_n */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            /* sum of base‑4 digits of i */
            idx_t x   = i;
            unsigned int sum = 0;
            while (x)
            {
                sum += (unsigned int)(x & 3U); /* x % 4 */
                x >>= 2U;                      /* x /= 4 */
            }

            /* compute sum^4 using seq_t arithmetic */
            seq_t d  = (seq_t)sum;
            seq_t d2 = d * d;
            seq_t d4 = d2 * d2;

            acc = acc + d4;
            cum[i] = acc;
        }
        max_n = n;
    }

    return cum[n];
}
