/*
 * A209824  Number of (n+1)X4 0..2 arrays with every 2X2 subblock having one
 *          or two distinct values, and new values 0..2 introduced in row
 *          major order.
 * https://oeis.org/A209824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 62*a(n-2) - 226*a(n-3) + 445*a(n-4) +
 *            475*a(n-5) - 811*a(n-6) + 29*a(n-7) + 147*a(n-8) - 20*a(n-9)
 *            Initial values (n = 1 … 9): a(1)=218, a(2)=4418, a(3)=90000,
 *            a(4)=1842509, a(5)=37744836, a(6)=773359089, a(7)=15846003427,
 *            a(8)=324684577671, a(9)=6652797311199 The recurrence is...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache allocator – static state is reused between calls. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_computed = 0;

    if (n == 0)
        return (seq_t)0;                 /* outside of offset – defensive */

    /* Resize cache if necessary. */
    if (cap <= (size_t)n)
    {
        size_t new_cap = (cap < 16) ? 32 : cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Cache the nine known initial values on first use. */
    if (max_computed < 9)
    {
        static const seq_t init[9] = {
            (seq_t)218ULL,
            (seq_t)4418ULL,
            (seq_t)90000ULL,
            (seq_t)1842509ULL,
            (seq_t)37744836ULL,
            (seq_t)773359089ULL,
            (seq_t)15846003427ULL,
            (seq_t)324684577671ULL,
            (seq_t)6652797311199ULL
        };
        for (idx_t i = max_computed + 1; i <= 9 && i <= n; ++i)
            cache[i] = init[i - 1];
        if (n >= 9)
            max_computed = 9;
        else
            max_computed = n;
    }

    /* Compute terms beyond the known initial ones using the recurrence. */
    if (max_computed < n)
    {
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            seq_t val = 0;
            /* Positive contributions */
            val += cache[i - 1] * (seq_t)24;
            val += cache[i - 4] * (seq_t)445;
            val += cache[i - 5] * (seq_t)475;
            val += cache[i - 7] * (seq_t)29;
            val += cache[i - 8] * (seq_t)147;
            /* Negative contributions */
            val -= cache[i - 2] * (seq_t)62;
            val -= cache[i - 3] * (seq_t)226;
            val -= cache[i - 6] * (seq_t)811;
            val -= cache[i - 9] * (seq_t)20;

            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
