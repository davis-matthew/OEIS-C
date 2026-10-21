/*
 * A224237  Decimal expansion of speed of light in miles per second.
 * https://oeis.org/A224237
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit of 299792458/(5280*0.3048)
 *            (starting a(6)=1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 6) return 0;                 /* indices below the offset are undefined */

    /* state persisted across calls */
    static bool init = false;
    static seq_t *digits = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static idx_t rem = 0;

    /* constants for the exact rational value */
    static const idx_t NUM = ((idx_t)299792458) * 10000U;
    static const idx_t DEN = ((idx_t)5280) * 3048U;   /* 5280 ft/mile * 3048 mm/ft */

    if (!init) {
        /* integer part and initial remainder */
        idx_t int_part = NUM / DEN;
        rem = NUM % DEN;

        /* extract the digits of the integer part (most significant first) */
        seq_t tmp_rev[64];
        size_t rev_len = 0;
        if (int_part == 0) {
            tmp_rev[rev_len++] = 0;
        } else {
            idx_t tmp = int_part;
            while (tmp > 0) {
                tmp_rev[rev_len++] = (seq_t)(tmp % 10U);
                tmp /= 10U;
            }
        }

        cap = rev_len;
        digits = (seq_t*)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < rev_len; ++i) {
            digits[i] = tmp_rev[rev_len - 1 - i];
        }
        sz = rev_len;
        init = true;
    }

    /* generate decimal digits on demand */
    while (sz <= n - 6) {
        if (sz == cap) {
            size_t new_cap = cap == 0 ? 32 : cap << 1;
            seq_t *tmp = (seq_t*)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failure – return 0 as safe fallback */
            digits = tmp;
            cap = new_cap;
        }
        rem *= 10U;
        seq_t digit = (seq_t)(rem / DEN);
        rem %= DEN;
        digits[sz++] = digit;
    }

    return digits[n - 6];
}
