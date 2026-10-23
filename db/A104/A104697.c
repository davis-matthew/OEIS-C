/*
 * A104697  Rearrangement of positive integers: change even digits d to 10-d.
 * https://oeis.org/A104697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = transform the decimal digits of n. Each digit d is
 *            replaced by - d if d is odd, - 10‑d if d is even (including 0).
 *            A zero digit becomes 0 with a carry of 1 added to the next more
 *            significant digit. The resulting decimal expansion is the value
 *            of a(n)
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* out of range; return 0 for safety */
        return (seq_t)0;

    /* maximum digits for 64‑bit integer ≈ 19, use a few more for safety */
    unsigned char out[32];
    size_t sz = 0;
    idx_t carry = 0;
    idx_t x = n;

    while (x > 0) {
        int d = (int)(x % 10);
        x /= 10;

        seq_t e;
        if (d == 0)            /* even zero */
            e = 0;
        else if ((d & 1) == 0) /* even non‑zero */
            e = (seq_t)(10 - d);
        else
            e = (seq_t)d;      /* odd digit unchanged */

        if (carry) {          /* carry from a preceding zero */
            e += carry;
            carry = 0;
        }

        if (e >= 10) {        /* propagate overflow carry */
            e -= 10;
            carry = 1;
        }

        if (d == 0)           /* zero contributes a carry to the next digit */
            carry = 1;

        out[sz++] = (unsigned char)e;
    }

    if (carry)                /* final carry becomes the most significant digit */
        out[sz++] = (unsigned char)carry;

    /* reconstruct the number from the collected digits */
    seq_t res = 0;
    for (size_t i = 0; i < sz; ++i)
        res = res * 10 + out[sz - 1 - i];

    return res;
}
