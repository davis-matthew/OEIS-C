/*
 * A192919  Constant term in the reduction by (x^2 -> x+1) of the polynomial
 *          F(n+4)*x^n, where F=A000045 (Fibonacci sequence).
 * https://oeis.org/A192919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n-1)*Fibonacci(n+4) Recurrence: a(0)=3; a(1)=0;
 *            a(2)=8; a(n)=2*a(n-1)+2*a(n-2)-a(n-3) for n≥3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* need to compute up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache)
        return 0;                 /* out of memory – caller will handle */

    /* initialise any newly added entries */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            new_cache[i] = 3;
        } else if (i == 1) {
            new_cache[i] = 0;
        } else if (i == 2) {
            new_cache[i] = 8;
        } else {
            /* compute using recurrence: a(n) = 2*a(n‑1)+2*a(n‑2)-a(n‑3) */
            new_cache[i] = new_cache[i-1] * 2
                         + new_cache[i-2] * 2
                         - new_cache[i-3];
        }
    }

    cache = new_cache;
    cache_size = new_size;
    return cache[n];
}
