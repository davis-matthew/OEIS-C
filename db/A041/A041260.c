/*
 * A041260  Numerators of continued fraction convergents to sqrt(142).
 * https://oeis.org/A041260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 286*a(n-4) - a(n-8) Initial values: a(0)=11, a(1)=12,
 *            a(2)=131, a(3)=143, a(4)=3277, a(5)=3420, a(6)=37477, a(7)=40897
 *            This function returns a(n) for any n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First eight terms are known explicitly. */
    static const seq_t init[8] = {
        (seq_t)11, (seq_t)12, (seq_t)131, (seq_t)143,
        (seq_t)3277, (seq_t)3420, (seq_t)37477, (seq_t)40897
    };

    /* Shortcut for indices less than 8. */
    if (n < 8) {
        return init[(size_t)n];
    }

    /* Ring buffer of the last eight terms. */
    seq_t buf[8];
    /* Initialize the buffer with the known first eight values. */
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms up to n. */
    for (idx_t i = 8; i <= n; ++i) {
        /* The recurrence only needs the term 4 and 8 steps back. */
        /* Index is modulo 8; use bitwise AND for speed. */
        buf[(size_t)(i & 7)] = (seq_t)286 * buf[(size_t)((i - 4) & 7)] - buf[(size_t)((i - 8) & 7)];
    }

    return buf[(size_t)(n & 7)];
}
