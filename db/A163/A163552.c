/*
 * A163552  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1)+28*a(n-2)+28*a(n-3)+28*a(n-4)-406*a(n-5) (for
 *            n>5). For n==5 the recurrence contains an additional +1 term.
 *            Initial values: a(0)=1, a(1)=30, a(2)=870, a(3)=25230,
 *            a(4)=731670
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* size of allocated cache */
    static idx_t  last   = 0;         /* highest index computed so far */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 6;                     /* indices 0..5 */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)870;
        cache[3] = (seq_t)25230;
        cache[4] = (seq_t)731670;

        /* a(5) uses the inhomogeneous recurrence with +1 */
        seq_t s = cache[4] + cache[3] + cache[2] + cache[1];
        seq_t val = s * (seq_t)28;
        val = val - (cache[0] * (seq_t)406);
        val = val + (seq_t)1;               /* +1 term for n==5 */
        cache[5] = val;

        last = 5;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t s = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
            seq_t val = s * (seq_t)28;
            val = val - (cache[i-5] * (seq_t)406);
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
