/*
 * A074025  Numbers k such that a triplewhist tournament TWh(k) exists.
 * https://oeis.org/A074025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, bref, nice
 * Formula:   a(n) = the n-th integer k satisfying k ≡ 0 or 1 (mod 4) and k ∉
 *            {5,9,12,13}. (k=17 is uncertain and omitted.)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Exceptions that are not in the sequence. */
    static const idx_t exceptions[] = {5, 9, 12, 13};
    const size_t excnt = sizeof(exceptions) / sizeof(exceptions[0]);

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t mod4 = k & 3u;          /* k modulo 4 */
        if (mod4 != 0 && mod4 != 1)   /* only 0 or 1 mod 4 are candidates */
            continue;

        /* skip known exceptional values */
        for (size_t i = 0; i < excnt; ++i) {
            if (k == exceptions[i])
                goto next_k;          /* k is an exception */
        }

        ++count;
        if (count == n)
            return (seq_t)k;

    next_k:
        ; /* nothing */
    }
}
