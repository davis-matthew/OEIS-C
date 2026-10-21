/*
 * A208568  Number of holes in n-th lower case letter of English alphabet (in
 *          hand-written form using a cursive script).
 * https://oeis.org/A208568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = holes[(n-1) % 26] where holes contains the number of
 *            holes in hand‑written cursive lower‑case English letters a..z
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t holes[26] = {
        1, /* a */
        1, /* b */
        0, /* c */
        1, /* d */
        1, /* e */
        2, /* f */
        2, /* g */
        1, /* h */
        0, /* i */
        1, /* j */
        2, /* k */
        1, /* l */
        0, /* m */
        0, /* n */
        1, /* o */
        1, /* p */
        2, /* q */
        0, /* r */
        1, /* s */
        0, /* t */
        0, /* u */
        0, /* v */
        0, /* w */
        0, /* x */
        1, /* y – handwritten cursive contains a loop */
        1  /* z – handwritten cursive contains a loop */
    };

    /* n >= 1; compute zero‑based index modulo 26 */
    size_t idx = (size_t)((n - 1) % 26);
    return holes[idx];
}
