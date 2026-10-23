/*
 * A021645  Decimal expansion of 1/641.
 * https://oeis.org/A021645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/641 (0‑based, a(0)=0)
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
    /* Static state to memoize previously computed digits */
    static seq_t *digits = NULL;      /* array of computed digits       */
    static size_t cap     = 0;        /* current allocated capacity     */
    static size_t len     = 0;        /* number of digits computed so far */
    static uint64_t rem   = 1;        /* remainder after last computed digit */

    /* Fast return if the requested digit is already cached */
    if (n < len)
        return digits[n];

    /* Ensure that the cache has some initial capacity */
    if (len == 0) {
        cap = 16;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!digits)              /* allocation failed – give up gracefully */
            return 0;
    }

    /* Compute digits up to and including index n */
    while (len <= n) {
        /* Long‑division step: multiply remainder by 10 and extract digit */
        rem *= 10U;
        seq_t d = (seq_t)(rem / 641U);
        rem  %= 641U;

        /* Grow the cache if necessary */
        if (len == cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(digits, cap * sizeof(seq_t));
            if (tmp) digits = tmp;      /* on failure we keep the old array */
        }

        digits[len++] = d;
    }

    return digits[n];
}
