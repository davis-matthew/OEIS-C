/*
 * A021091  Decimal expansion of 1/87.
 * https://oeis.org/A021091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/87) mod 10 Computes the nth decimal digit
 *            of 1/87 (with a(0)=0 integer part)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache of previously computed digits. */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* number of digits cached (cache[0..computed-1]) */
    static seq_t rem = 1;        /* remainder before the next digit extraction */

    /* Start fresh if nothing has been computed yet. */
    if (computed == 0)
        rem = 1;

    if (n >= computed) {
        /* Extend the cache to hold digits up to index n. */
        cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = computed; i <= n; ++i) {
            rem = rem * 10;             /* bring down next digit */
            seq_t digit = rem / 87;      /* integer part of the division */
            rem = rem % 87;              /* new remainder for next step */
            cache[i] = digit;
        }
        computed = n + 1;
    }

    return cache[n];
}
