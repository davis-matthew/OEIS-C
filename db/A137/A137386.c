/*
 * A137386  Numbers that when mirrored vertically are still valid characters.
 *          Only applies when numbers are scribed in Arabic numerals.
 * https://oeis.org/A137386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137386: Numbers composed only of digits 0,1,3,6,8,9.
   Equivalent to interpreting n‑1 in base‑6, mapping digits
   0→0, 1→1, 2→3, 3→6, 4→8, 5→9, and then treating the result as a base‑10 number. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* offset index: n starts at 1 */
    idx_t k = n - 1;

    /* mapping from base‑6 digit to allowed decimal digit */
    static const seq_t map[6] = {
        (seq_t)0, (seq_t)1, (seq_t)3,
        (seq_t)6, (seq_t)8, (seq_t)9
    };

    /* collect digits in reverse order; 64‑bit n needs less than 32 base‑6 digits. */
    seq_t stack[32];
    size_t p = 0;

    /* handle the case k == 0 explicitly so that the result is 0. */
    do {
        stack[p++] = map[k % 6];
        k /= 6;
    } while (k != 0);

    seq_t res = 0;
    for (size_t i = p; i-- > 0; ) {
        res = res * 10 + stack[i];
    }
    return res;
}
