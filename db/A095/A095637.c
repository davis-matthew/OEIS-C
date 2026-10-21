/*
 * A095637  a(n) = 123 written in base 11 - n.
 * https://oeis.org/A095637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = decimal number obtained by writing 123 in base (11‑n) and then
   interpreting that digit string as a decimal integer.  For n > 10 the base
   would be ≤ 0, which is undefined for this sequence.  For n = 10 (base 1)
   we use the unary representation of 123 (a string of 123 ones). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the base 11 - n; use signed to allow negative values */
    long long base = (long long)11 - (long long)n;

    /* undefined when the base ≤ 0 */
    if (base <= 0)
        return (seq_t)0;

    /* special case: base 1 → unary representation */
    if (base == 1)
    {
        seq_t res = 0;
        for (int i = 0; i < 123; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* standard base conversion for base ≥ 2 */
    int b = (int)base;          /* b fits in an int: 2 ≤ b ≤ 11 */
    int v = 123;                /* the constant to convert */
    int digits[32];
    int cnt = 0;

    /* extract digits in little‑endian order */
    while (v > 0)
    {
        digits[cnt++] = v % b;
        v /= b;
    }

    /* interpret the digit string as decimal, most‑significant first */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i)
        res = res * 10 + (seq_t)digits[i];

    return res;
}
