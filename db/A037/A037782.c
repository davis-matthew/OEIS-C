/*
 * A037782  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,2,0.
 * https://oeis.org/A037782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A037782: Base 7 digits are, in order, the first n terms of the periodic sequence with initial period 3,1,2,0.
 * Recurrence: a(0)=0, a(1)=3, a(2)=22, a(3)=156, a(4)=1092
 *              a(n) = 7*a(n-1) + a(n-4) - 7*a(n-5)   for n ≥ 5
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, a(0) is used only internally */

    size_t target = (size_t)n;           /* number of terms needed (1-based) */

    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* cache[0] = a(1), cache[1] = a(2), … */
    static size_t allocated = 0;         /* current allocation size (in entries) */
    static size_t computed  = 0;         /* number of cache entries already computed */

    /* grow allocation if necessary */
    if (allocated < target) {
        size_t new_alloc = allocated ? allocated << 1 : 5;
        while (new_alloc < target) new_alloc <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) exit(1);               /* allocation failure */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute missing terms */
    if (computed < target) {
        for (size_t i = computed; i < target; ++i) {
            /* i is 0‑based: i==0 → a(1), i==1 → a(2), … */
            if (i == 0)           cache[0] = 3;
            else if (i == 1)       cache[1] = 22;
            else if (i == 2)       cache[2] = 156;
            else if (i == 3)       cache[3] = 1092;
            else if (i == 4) {     /* a(5): a(0)=0 */
                cache[4] = 7 * cache[3] + cache[0];
            } else {
                cache[i] = 7 * cache[i - 1] + cache[i - 4] - (7 * cache[i - 5]);
            }
        }
        computed = target;
    }

    return cache[target - 1];             /* return a(n) */
}
