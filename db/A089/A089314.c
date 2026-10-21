/*
 * A089314  Sum of all digits in all even numbers from 0 to 444...4 (with n
 *          4's).
 * https://oeis.org/A089314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (9*(9*10^n+28)*n-64*(10^n-1))/81
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 10^n in the same type as the sequence value */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= 10;

    /* For small seq_t sizes use a 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn      = (unsigned __int128)n;
        unsigned __int128 p10     = (unsigned __int128)pow10;
        unsigned __int128 num     = (unsigned __int128)9 *
                                   ((unsigned __int128)9 * p10 + 28) * nn
                                   - (unsigned __int128)64 * (p10 - 1);
        return (seq_t)(num / 81);
    }

    /* For arbitrary‑precision seq_t the scalar operators are sufficient. */
    seq_t num = 9 * (9 * pow10 + 28) * (seq_t)n - 64 * (pow10 - 1);
    return num / 81;
}
