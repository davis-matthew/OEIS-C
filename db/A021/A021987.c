/*
 * A021987  Decimal expansion of 1/983.
 * https://oeis.org/A021987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n of 1/983 (obtained by iterative long
 *            division): r_0 = 1; for i ≥ 0: digit_i = (10 * r_i) / 983;
 *            r_{i+1} = (10 * r_i) % 983; The function caches all computed
 *            digits so each digit is calculated at most once
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *digits = NULL;          /* array of digits 0..9 */
    static size_t cache_len = 0;          /* number of digits stored */
    static size_t cache_cap = 0;          /* allocated capacity */
    static idx_t remainder = 1;           /* current remainder for long division */

    /* grow the cache so that digits[0..n] exist */
    if (n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* OOM – abort program */
                abort();
            }
            digits = tmp;
            cache_cap = new_cap;
        }
        /* compute missing digits */
        for (; cache_len <= (size_t)n; ++cache_len) {
            /* next digit and new remainder */
            idx_t r10 = remainder * 10u;
            digits[cache_len] = (seq_t)(r10 / 983u);
            remainder = r10 % 983u;
        }
    }

    return digits[(size_t)n];
}
