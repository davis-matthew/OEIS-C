/*
 * A043820  Number of runs in the base 3 representation of n is congruent to 5
 *          mod 10.
 * https://oeis.org/A043820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) – nth integer whose number of runs in the base‑3
 * representation is congruent to 5 mod 10.
 * The function builds the sequence incrementally, keeping a static
 * cache so that arbitrary order of calls is supported.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL; /* array of computed terms   */
    static size_t cache_len = 0;/* number of terms stored    */
    static size_t cache_cap = 0;/* capacity of the array     */
    static idx_t current = 1;   /* next integer to test      */

    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 */

    while (cache_len < n)
    {
        /* test the next integer */
        idx_t m = current++;
        /* count runs in base‑3 representation of m */
        int runs = 1;             /* at least one run when m > 0  */
        int prev = m % 3;        /* least significant digit     */
        idx_t t = m / 3;
        while (t != 0)
        {
            int d = t % 3;
            if (d != prev)
            {
                ++runs;
                prev = d;
            }
            t /= 3;
        }

        if ((runs % 10) == 5)
        {
            /* store the integer in the cache */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* If realloc fails, terminate (sequence not needed further) */
                if (tmp == NULL) return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
