/*
 * A044334  Numbers n such that string 0,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044334: the nth integer n such that
 * the decimal representation of n contains the substring "02" while
 * that of n-1 does not.  The implementation searches forward from 1
 * and uses get_digits to examine decimal digits, which works for
 * both fixed‑width and arbitrary‑precision seq_t. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"

/* Test whether a decimal number contains the consecutive digits 0 followed by 2. */
static bool has02(seq_t v)
{
    if (v == 0)          /* single digit 0 cannot contain "02" */
        return false;

    /* Use a buffer large enough for the greatest number we are likely to
     * encounter in the requested range (n ≤ 10000).  It is safe for larger
     * numbers as well because seq_t may represent arbitrarily large values. */
    uint8_t buf[1024];
    size_t len;

    get_digits(v, buf, &len);          /* buf holds digits little‑endian */
    for (size_t i = 1; i < len; ++i)
        if (buf[i] == 0 && buf[i - 1] == 2)
            return true;

    return false;
}

/* OEIS sequence A044334 */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, return 0 for safety */
        return 0;

    seq_t x = 1;       /* candidate number */
    idx_t count = 0;   /* how many qualifying numbers found so far */

    for (;;) {
        if (has02(x) && !has02(x - 1)) {
            ++count;
            if (count == n)
                return x; /* nth qualifying number found */
        }
        ++x;
    }
}
