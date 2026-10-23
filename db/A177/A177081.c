/*
 * A177081  Numbers of the form x^x, where x equals the number of digits in
 *          the number.
 * https://oeis.org/A177081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref, fini, full
 * Formula:   a(n) is the nth integer m such that there exists an integer x
 *            with m = x^x and the decimal digit length of m equals x
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Count how many valid numbers we have seen. */
    idx_t found = 0;

    /* Only the first few x can satisfy digit_len(x^x) == x.
       For x ≥ 10 the digit length is always greater than x.
       Hence we loop only up to a small bound and break once
       we are certain no further solutions exist. */
    for (idx_t x = 1; x <= 20; ++x)
    {
        seq_t val = big_pow((seq_t)x, (seq_t)x);
        int d = digit_len(val);

        if (d == (int)x)
        {
            ++found;
            if (found == n)
                return val;
        }

        /* From x = 10 onwards the digit length exceeds x,
           so no further solutions exist. */
        if (x >= 10 && d > (int)x)
            break;
    }

    /* If the requested index is beyond the known terms, return 0. */
    return (seq_t)0;
}
