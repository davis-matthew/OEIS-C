/*
 * A084518  Partial sums of A084519. Positions of ones in the first
 *          differences of A084516.
 * https://oeis.org/A084518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) satisfies the linear recurrence
   a(n) = 4*a(n-1) - a(n-2) - 2*a(n-4)   for n ≥ 5
   with initial values
   a(1)=1, a(2)=2, a(3)=5, a(4)=18.
   The recurrence follows from the empirical generating function
   x*(1‑2x‑2x²)/((1‑x)*(1‑3x‑2x²‑2x³)). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order access */
    static seq_t *cache = NULL;
    static idx_t cached = 0;       /* largest index that is already stored */

    /* if n <= 0 do not compute (undefined); assume caller passes n>=1 */
    if (n == 0) return 0;

    /* extend cache if necessary */
    if (n > cached)
    {
        idx_t newsize = n + 1;               /* indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed – return 0 */
        cache = tmp;

        /* compute new entries starting from cached+1 up to n */
        for (idx_t i = cached + 1; i <= n; ++i)
        {
            seq_t val;
            if (i == 1)           val = 1;
            else if (i == 2)      val = 2;
            else if (i == 3)      val = 5;
            else if (i == 4)      val = 18;
            else
            {
                /* 4*a(i-1) - a(i-2) - 2*a(i-4) */
                seq_t mul4  = 4 * cache[i-1];
                seq_t mul2  = 2 * cache[i-4];
                val = mul4 - cache[i-2] - mul2;
            }
            cache[i] = val;
        }
        cached = n;
    }
    return cache[n];
}
