/*
 * A037629  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0.
 * https://oeis.org/A037629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-3) - 8*a(n-4) for n>=5 initial terms:
 *            a(1)=2, a(2)=19, a(3)=152, a(4)=1218
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed = 0;  /* number of computed entries */

    /* Ensure storage for at least n terms */
    if (n > capacity) {
        idx_t new_cap = capacity ? (capacity << 1) : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[i-1] = (seq_t)2;
        else if (i == 2)
            cache[i-1] = (seq_t)19;
        else if (i == 3)
            cache[i-1] = (seq_t)152;
        else if (i == 4)
            cache[i-1] = (seq_t)1218;
        else {
            seq_t term = (seq_t)8 * cache[i-2] + cache[i-4] - (seq_t)8 * cache[i-5];
            cache[i-1] = term;
        }
    }
    if (computed < n) computed = n;
    return cache[n-1];
}
