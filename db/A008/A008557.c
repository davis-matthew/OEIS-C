/*
 * A008557  Repeatedly convert from decimal to octal.
 * https://oeis.org/A008557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(1) = 8 * a(n+1) = decimal value of the octal representation of
 *            a(n) * (i.e. interpret the octal digits of a(n) as a decimal
 *            number)
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 30, u64 n <= 38, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms; index 0 holds a(1) */
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t   len   = 0;         /* number of values currently stored */
    static size_t  cap   = 0;         /* allocated capacity of cache */

    if (n == 0) return 0;            /* undefined for offset <1; safe fallback */

    /* extend cache to at least n terms */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap : 64;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* assume allocation succeeds for contest environment */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialize first term if cache empty */
    if (len == 0) {
        cache[0] = (seq_t)8;
        len = 1;
    }

    /* compute until reaching desired n */
    for (idx_t i = len; i < n; ++i) {
        seq_t cur = cache[i - 1];      /* a(i) */

        /* count digits in octal representation of cur */
        seq_t tmp = cur;
        idx_t oct_len = 0;
        while (tmp > 0) {
            tmp /= (seq_t)8;
            ++oct_len;
        }

        /* allocate buffer for octal digits (least significant first) */
        uint8_t *digits = NULL;
        if (oct_len > 0) {
            digits = (uint8_t*)malloc((size_t)oct_len);
        }

        /* extract octal digits */
        tmp = cur;
        for (idx_t k = oct_len; k > 0; --k) {
            digits[k - 1] = (uint8_t)(tmp % (seq_t)8);
            tmp /= (seq_t)8;
        }

        /* convert octal digits to decimal number */
        seq_t next = 0;
        for (idx_t k = 0; k < oct_len; ++k) {
            next = next * (seq_t)10 + digits[k];
        }

        /* clean up */
        if (digits) free(digits);

        /* store result */
        cache[i] = next;
    }

    len = n;                      /* now we have terms up to a(n) */
    return cache[n - 1];
}
