/*
 * A060824  4-wave sequence beginning with 2s.
 * https://oeis.org/A060824
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A038197(n-2). For n ≥ 14: a(n) = 2*a(n-3) + 3*a(n-6) –
 *            a(n-9) – a(n-12) Initial values: a(2)=2, a(3)=2, a(4)=2, a(5)=2,
 *            a(6)=4, a(7)=6, a(8)=8, a(9)=14, a(10)=18, a(11)=20, a(12)=38,
 *            a(13)=52. Offset: 2 (a(2) is the first defined term)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(n) is defined for n ≥ 2.  For n < 2 behavior is irrelevant. */
    if (n < 2) return (seq_t)0;

    static seq_t *cache     = NULL;       /* cache of computed terms       */
    static idx_t cache_size = 0;          /* current allocated size (in terms)   */
    static idx_t max_done   = 1;          /* highest index that has been computed */

    /* Grow the cache if necessary.  We keep indices 0 … n inclusive. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n. */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        switch (i) {
            case 2:  cache[i] = (seq_t)2; break;
            case 3:  cache[i] = (seq_t)2; break;
            case 4:  cache[i] = (seq_t)2; break;
            case 5:  cache[i] = (seq_t)2; break;
            case 6:  cache[i] = (seq_t)4; break;
            case 7:  cache[i] = (seq_t)6; break;
            case 8:  cache[i] = (seq_t)8; break;
            case 9:  cache[i] = (seq_t)14; break;
            case 10: cache[i] = (seq_t)18; break;
            case 11: cache[i] = (seq_t)20; break;
            case 12: cache[i] = (seq_t)38; break;
            case 13: cache[i] = (seq_t)52; break;
            default: /* Recurrence for i ≥ 14 */
                cache[i] = 2 * cache[i - 3] + 3 * cache[i - 6]
                          - cache[i - 9] - cache[i - 12];
                break;
        }
    }

    max_done = n > max_done ? n : max_done;
    return cache[n];
}
