/*
 * A195572  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 1/5.
 * https://oeis.org/A195572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 101*a(n-3) + 101*a(n-6) - a(n-9) with initial terms:
 *            a(1)=0; a(2)=9; a(3)=11; a(4)=20; a(5)=819; a(6)=1221;
 *            a(7)=2020; a(8)=83629; a(9)=124431
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size (index 0 unused) */
    static idx_t  last = 0;         /* highest n for which cache[n] is valid */

    if (n == 0) return 0;           /* not used for OEIS offset 1 */

    /* Allocate/expand cache if necessary. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;   /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as fallback */
            free(cache);
            cache = NULL;
            cap = 0;
            last = 0;
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the 9 initial terms whenever we have space for them.
       This re‑writes the same values but is harmless. */
    if (n >= 1) cache[1] = 0;
    if (n >= 2) cache[2] = 9;
    if (n >= 3) cache[3] = 11;
    if (n >= 4) cache[4] = 20;
    if (n >= 5) cache[5] = 819;
    if (n >= 6) cache[6] = 1221;
    if (n >= 7) cache[7] = 2020;
    if (n >= 8) cache[8] = 83629;
    if (n >= 9) cache[9] = 124431;

    /* If the answer is already computed, return it. */
    if (n <= last) return cache[n];

    /* Compute missing terms only once, from the smallest pending index. */
    idx_t start = (last >= 9 ? last : 9);
    for (idx_t i = start + 1; i <= n; ++i) {
        /* Recurrence is valid only for n >=10 */
        cache[i] = (seq_t)101 * cache[i-3] +
                   (seq_t)101 * cache[i-6] - cache[i-9];
    }
    last = n;
    return cache[n];
}
