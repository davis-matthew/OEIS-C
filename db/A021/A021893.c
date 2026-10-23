/*
 * A021893  Decimal expansion of 1/889.
 * https://oeis.org/A021893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10 * r_n / 889), r_0 = 1, r_{k+1} = (10 * r_k) %
 *            889. This yields the decimal digits of 1/889
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: cached digits, allocated capacity, largest index computed so far,
       and the remainder after the last computed digit (initially r_0 = 1). */
    static seq_t *digits      = NULL;   /* array of digits a(0), a(1), ... */
    static size_t cache_len   = 0;      /* allocated number of entries */
    static idx_t last_index   = (idx_t)-1; /* last index computed; -1 = none yet */
    static idx_t rem          = 1;          /* current remainder r_{last_index+1} */

    /* If we already have the requested index cached, just return it. */
    if (last_index != (idx_t)-1 && n <= last_index)
        return digits[n];

    /* Allocate or grow the cache to accommodate index n. */
    if (cache_len < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(digits, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();                     /* unlikely in tests */
        digits = tmp;
        cache_len = (size_t)(n + 1);
    }

    /* Generate missing digits up to index n, updating the remainder. */
    for (idx_t i = (last_index == (idx_t)-1) ? 0 : last_index + 1; i <= n; ++i) {
        rem *= 10;                          /* bring next decimal place */
        seq_t d = (seq_t)(rem / 889);        /* digit at position i */
        digits[i] = d;
        rem %= 889;                          /* remainder for next step */
    }

    last_index = n;                         /* we now have digits up to n */
    return digits[n];
}
