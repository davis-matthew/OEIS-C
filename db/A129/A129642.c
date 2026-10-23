/*
 * A129642  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+457)^2 = y^2.
 * https://oeis.org/A129642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3)-a(n-6)+914 for n>6; a(1)=0, a(2)=348, a(3)=495,
 *            a(4)=1371, a(5)=3255, a(6)=4088
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 28, u64 n <= 66, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* Initial values for n = 1..6. */
    static const seq_t init[6] = {0, 348, 495, 1371, 3255, 4088};

    if (n == 0) return 0; /* safety for invalid input */

    /* Ensure the cache can hold up to n terms. */
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure – terminate */
        cache = tmp;
        /* Initialize newly allocated entries. */
        for (idx_t i = cache_size; i < n; ++i) {
            if (i < 6) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i+1) = 6*a(i-2) - a(i-5) + 914  (since i is 0‑index) */
                cache[i] = 6 * cache[i - 3] - cache[i - 6] + 914;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
