/*
 * A192798  Constant term in the reduction of the n-th Fibonacci polynomial by
 *          x^3->x^2+2. See Comments.
 * https://oeis.org/A192798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+3*a(n-2)-3*a(n-4)+a(n-5)+a(n-6) Initial values:
 *            a(1)=1, a(2)=0, a(3)=1, a(4)=2, a(5)=3, a(6)=10
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t max_n = 0;

/* Ensure that cache[1..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (n <= max_n)
        return;

    idx_t new_size = n + 1;                     /* index 0 unused */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return;   /* In a real program, handle allocation failure */

    /* Initialize new entries, extending from max_n+1 to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 6) {
            switch (i) {
                case 1: new_cache[i] = (seq_t)1; break;
                case 2: new_cache[i] = (seq_t)0; break;
                case 3: new_cache[i] = (seq_t)1; break;
                case 4: new_cache[i] = (seq_t)2; break;
                case 5: new_cache[i] = (seq_t)3; break;
                case 6: new_cache[i] = (seq_t)10; break;
            }
        } else {
            new_cache[i] = new_cache[i-1]
                           + (seq_t)3 * new_cache[i-2]
                           - (seq_t)3 * new_cache[i-4]
                           + new_cache[i-5]
                           + new_cache[i-6];
        }
    }

    cache = new_cache;
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
