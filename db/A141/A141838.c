/*
 * A141838  a(n) = first term that can be reduced in n steps via repeated
 *          interpretation of a(n) as a base b+1 number where b is the largest
 *          digit of a(n), such that b is always 4 so that each interpretation
 *          is base 5. Terms already fully reduced (i.e., single digits) are
 *          excluded.
 * https://oeis.org/A141838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   a(n) = minimal term that can be reduced in exactly n steps by
 *            repeatedly interpreting the term as a base‑5 number. (Terms that
 *            have only one digit are excluded.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: compute the next term in the sequence.
   given y, interpret the decimal digits of y as a base‑5 number
   and return the resulting decimal integer. */
static seq_t next_step(seq_t y)
{
    /* Maximum number of base‑5 digits that can appear for n up to 10000
       (a few digits more than n for safety).  This buffer is reused
       for every call to avoid dynamic allocation. */
    static uint8_t digits[40000];
    idx_t count = 0;
    seq_t tmp = y;

    /* Extract base‑5 digits (least significant first). */
    while (tmp > 0) {
        digits[count++] = (uint8_t)(tmp % 5);
        tmp /= 5;
    }

    /* No digits means y == 0, which does not occur in this sequence. */
    if (count == 0) return 0;

    /* Build the decimal integer from most significant to least. */
    seq_t res = 0;
    for (idx_t i = count; i > 0; --i) {
        res = res * 10 + digits[i - 1];
    }
    return res;
}

/* Public sequence generator.  Uses static memoisation so that
   calls with arbitrary order of n remain efficient. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return 0;          /* offset starts at 1; 0 is unused */

    if (n > cache_len) {
        /* Extend the cache to at least n elements (1‑based). */
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!new_cache) abort();

        /* Generate new terms from the last cached value. */
        seq_t val;
        if (cache_len == 0) {
            val = 9;              /* Starting point before first f‑step. */
        } else {
            val = cache[cache_len];
        }

        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            val = next_step(val);
            new_cache[i] = val;
        }

        cache = new_cache;
        cache_len = new_len;
    }
    return cache[n];
}
