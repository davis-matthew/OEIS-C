/*
 * A076466  a(1)=1 a(n) = a(n-1) + ((-1)^a(n-1)*a(n-1)) mod n.
 * https://oeis.org/A076466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1 a(n) = a(n-1) + ((-1)^a(n-1) * a(n-1)) mod n (n ≥ 2)
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1] holds a(1) */
    static idx_t cached_max = 0;  /* maximum n for which cache is filled */

    if (n == 0) return 0;         /* out of range – not used by tests */

    /* Ensure cache is large enough and fill missing values */
    if (n > cached_max) {
        /* Resize cache to hold indices up to n */
        size_t new_size = (size_t)n + 1;                 /* +1 because we ignore 0 */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* If we have no entries yet, set a(1) */
        if (cached_max < 1) {
            cache[1] = (seq_t)1;
            cached_max = 1;
        }

        /* Compute values from cached_max+1 up to n */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];                       /* a(i-1) */
            seq_t m = prev % (seq_t)i;                      /* remainder of a(i-1) modulo i */

            seq_t add;
            if ((prev & (seq_t)1) == 0)                     /* a(i-1) even */
                add = m;                                    /* (+1)*m */
            else
                add = (m == 0) ? 0 : ((seq_t)i - m);        /* (-1)*m ≡ i-m (mod i) */

            cache[i] = prev + add;                          /* a(i) */
        }
        cached_max = n;
    }

    return cache[n];
}
