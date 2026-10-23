/*
 * A021195  Decimal expansion of 1/191.
 * https://oeis.org/A021195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n‑th decimal digit of 1/191 (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of digits computed so far */
    static seq_t *digits = NULL;
    static size_t len   = 0;   /* number of digits currently cached */
    static size_t cap   = 0;   /* current capacity of the array    */
    static uint64_t rem = 1;   /* remainder before generating digit */

    /* extend the cache up to index n (inclusive) if needed */
    if ((size_t)n >= len) {
        size_t need = (size_t)n + 1;        /* digits required */

        /* grow the buffer if it is too small */
        if (need > cap) {
            size_t new_cap = cap ? (cap << 1) : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure -> zero */
            digits = tmp;
            cap    = new_cap;
        }

        /* generate the missing digits */
        for (size_t i = len; i < need; ++i) {
            rem *= 10;                       /* bring next decimal place in */
            seq_t d = (seq_t)(rem / 191);    /* high‑order digit of the next step */
            rem   %= 191;                    /* new remainder modulo 191   */
            digits[i] = d;
        }
        len = need;
    }
    return digits[(size_t)n];
}
