/*
 * A076086  a(1) = 1; a(n) is the smallest multiple of a(n-1) not divisible by
 *          10 which is greater than the digit reversal of a(n-1). In case
 *          R(a(n-1)) < a(n-1) then a(n) = 2*a(n-1).
 * https://oeis.org/A076086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1. For n > 1 let r = reverse(a(n‑1)). • If r < a(n‑1)
 *            then a(n) = 2*a(n‑1). • Otherwise a(n) is the smallest multiple
 *            of a(n‑1) that is > r and not divisible by 10
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t reverse_seq_t(seq_t x)
{
    seq_t rev = (seq_t)0;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)10;
        rev = rev * (seq_t)10 + d;
        x   = x / (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* cache[0] is unused; valid indices start at 1 */
    static seq_t *cache = NULL;
    static idx_t  last   = 0;        /* highest index already computed   */
    static size_t capacity = 0;     /* allocated size of the cache array */

    if (n < (idx_t)1)   /* sequence starts at offset 1 */
        return (seq_t)0;

    /* Allocate / grow cache as needed (C++ requires explicit cast) */
    if (n > last) {
        if (capacity == 0) capacity = 4;
        while (capacity <= (size_t)n) capacity <<= 1;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        /* Compute new terms */
        for (idx_t i = last + (idx_t)1; i <= n; ++i) {
            if (i == (idx_t)1) {
                cache[(size_t)i] = (seq_t)1;
                continue;
            }
            seq_t prev = cache[(size_t)(i - (idx_t)1)];
            seq_t rev  = reverse_seq_t(prev);

            if (rev < prev) {
                cache[(size_t)i] = prev * (seq_t)2;
            } else {
                /* Find smallest k such that prev*k > rev and prev*k % 10 != 0 */
                seq_t k = rev / prev + (seq_t)1;     /* minimal k with prev*k > rev */
                while ((prev * k) % (seq_t)10 == (seq_t)0)
                    ++k;
                cache[(size_t)i] = prev * k;
            }
        }
        last = n;
    }
    return cache[(size_t)n];
}
