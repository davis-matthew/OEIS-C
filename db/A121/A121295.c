/*
 * A121295  Descending dungeons: for definition see Comments lines.
 * https://oeis.org/A121295
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpretation of the decimal digits of n in base a(n-1)
 *            with a(10) = 10 and for n ≥ 11: a(n) = Σ_{i=0}^{m-1}
 *            a(n-1)^(m-1-i) · d_(m-i) where n = d_m…d_2d_1 in decimal
 * Range:     u8 n <= 21, u16 n <= 29, u32 n <= 39, u64 n <= 54, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* the sequence is defined for n ≥ 10; below the offset behaviour does not matter */
    if (n < 10)
        return 0;   /* undefined but unused in tests */

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;                     /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache[n] != 0)
        return cache[n];   /* already computed */

    /* base case */
    if (n == 10) {
        cache[n] = 10;
        return cache[n];
    }

    /* recursively obtain the base for this step */
    seq_t base = a(n - 1);

    /* interpret the decimal digits of n in that base */
    seq_t result = 0;
    seq_t power  = 1;   /* base^0 */
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned digit = tmp % 10;                /* 0‑9 */
        result = result + power * (seq_t)digit;
        power  = power * base;
        tmp   /= 10;
    }

    cache[n] = result;
    return result;
}
