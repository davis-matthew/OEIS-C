/*
 * A048390  Digits d in decimal expansion of n replaced with d^3.
 * https://oeis.org/A048390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n ≥ 0, write its decimal digits dₖ…d₀ and replace each digit
 *            by dᵢ³. The decimal strings of the cubes are concatenated to
 *            form a(new). Example: n = 14 → digits 1,4 → 1³ = 1, 4³ = 64 →
 *            a(14) = 164
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cubes of 0–9 */
    static const seq_t cube[10] = {
        (seq_t)0,  (seq_t)1,  (seq_t)8,  (seq_t)27,
        (seq_t)64, (seq_t)125,(seq_t)216,(seq_t)343,
        (seq_t)512,(seq_t)729
    };
    /* Decimal length of each cube */
    static const unsigned char cube_len[10] = {
        1,1,1,2,2,3,3,3,3,3
    };
    /* 10⁰ … 10³ as seq_t */
    static const seq_t pow10[4] = {
        (seq_t)1, (seq_t)10, (seq_t)100, (seq_t)1000
    };

    if (n == 0)
        return (seq_t)0;

    /* Extract digits of n in reverse order */
    idx_t tmp = n;
    unsigned char rev_digits[32];
    size_t rev_len = 0;
    while (tmp > 0) {
        rev_digits[rev_len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Build result from most significant digit to least */
    seq_t result = (seq_t)0;
    for (size_t i = rev_len; i > 0; --i) {
        unsigned char d = rev_digits[i - 1];
        seq_t val = cube[d];
        result = result * pow10[cube_len[d]] + val;
    }

    return result;
}
