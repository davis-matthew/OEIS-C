/*
 * A021859  Decimal expansion of 1/855.
 * https://oeis.org/A021859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of 1/855 (a(0) is the first digit
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed digits, plus current remainder. */
    static seq_t *cache = NULL;        /* array of digits a(0), a(1), ... */
    static idx_t  cache_cnt = 0;       /* number of digits stored currently   */
    static idx_t  cache_cap = 0;       /* allocated capacity                   */
    static uint64_t rem = 1;           /* remainder before computing a digit  */

    /* Grow cache until it holds a(n). */
    while (cache_cnt <= n)
    {
        /* Expand cache capacity if necessary. */
        if (cache_cnt == cache_cap)
        {
            cache_cap = (cache_cap == 0 ? 1 : cache_cap * 2);
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;  /* allocation failure, return 0 */
        }

        /* Long‑division step: produce next decimal digit. */
        rem *= 10ULL;                       /* bring down next digit of 1 */
        seq_t digit = (seq_t)(rem / 855ULL); /* 855 fits in 64 bits */
        rem   %= 855ULL;                    /* remainder for next step */

        cache[cache_cnt] = digit;
        cache_cnt++;
    }

    return cache[n];
}
