/*
 * A008566  Digits of powers of 5.
 * https://oeis.org/A008566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3580, u16 n <= 3580, u32 n <= 3580, u64 n <= 3580,
 *            u128 n <= 3580, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n+1)-th digit in the infinite sequence formed by
 * concatenating the decimal expansions of successive powers of 5.
 * That is, write 1, 5, 25, 125, 625, … one after another;
 * then a(0) = 1, a(1) = 5, a(2) = 2, a(3) = 5, a(4) = 1, ….
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  decimal digits of the current power of 5,
     *  stored least-significant first (digits[0] = least significant).
     */
    uint8_t digits[2064];
    size_t sz = 1;
    digits[0] = 1;          /* 5^0 = 1 */

    idx_t remaining = n;

    for (;;)
    {
        /* if the desired digit lies within the current power,
           return it (most‑significant first). */
        if (remaining < (idx_t)sz)
        {
            uint8_t d = digits[sz - 1 - remaining];
            return (seq_t)d;
        }

        remaining -= sz;    /* skip all digits of this power */

        /* multiply the current power by 5 -> next power of 5 */
        unsigned carry = 0;
        for (size_t i = 0; i < sz; ++i)
        {
            unsigned prod = digits[i] * 5 + carry;
            digits[i]   = prod % 10;
            carry       = prod / 10;
        }
        while (carry)
        {
            digits[sz++] = carry % 10;
            carry       /= 10;
        }
    }
}
