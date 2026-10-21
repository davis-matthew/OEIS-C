/*
 * A208030  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208030: Number of 5×n 0..1 arrays avoiding
 *  horizontally  (0 0 0 , 0 0 1) and vertically (0 0 0 , 1 0 1).
 *
 *  Empirical recurrence
 *      a(n) = 3*a(n-1) + 19*a(n-2) - 9*a(n-3) - 42*a(n-4)
 *           + 31*a(n-5) + 3*a(n-6) - 4*a(n-7)
 *  with initial values
 *      a(1)=13, a(2)=169, a(3)=741, a(4)=4823,
 *      a(5)=26845, a(6)=158847, a(7)=917293
 *
 *  The function memoises computed values in a static dynamic array so
 *  that calls in any order remain fast (O(n) overall for the largest
 *  argument seen).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset is 1: a(1) is the first term */
    if (n == 0)
        return 0;

    /* static memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t  max_n  = 0;   /* largest n for which cache[n] is valid   */
    static idx_t  cap    = 0;   /* current allocated size (in elements)    */

    /* First time: allocate room for the 7 base terms */
    if (cache == NULL) {
        /* reserve at least indices 1..7; use index 0 as dummy */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* memory allocation failure – return 0 */

        /* initialise the first 7 terms */
        cache[0] = 0;            /* unused */
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)169;
        cache[3] = (seq_t)741;
        cache[4] = (seq_t)4823;
        cache[5] = (seq_t)26845;
        cache[6] = (seq_t)158847;
        cache[7] = (seq_t)917293;
        max_n   = 7;
    }

    /* grow the buffer if necessary to hold up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – keep old buffer */
            return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* split positive and negative contributions to avoid
             * unsigned underflow when subtracting */
            seq_t pos = (seq_t)3 * cache[i-1]
                        + (seq_t)19 * cache[i-2]
                        + (seq_t)31 * cache[i-5]
                        + (seq_t)3  * cache[i-6];

            seq_t neg = (seq_t)9  * cache[i-3]
                        + (seq_t)42 * cache[i-4]
                        + (seq_t)4  * cache[i-7];

            cache[i] = pos - neg;   /* final result is non‑negative */
        }
        max_n = n;
    }

    return cache[n];
}
