/*
 * A230979  Decimal expansion of the time taken by light to travel one
 *          astronomical unit in a vacuum, measured in seconds.
 * https://oeis.org/A230979
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 149597870700 / 299792458
   (i.e., the light‑travel time for one astronomical unit, m/s)
   with OEIS offset 3: the first term a(3) corresponds to the first digit. */

#include <stdint.h>
#include "../utils/pow10.h"

/* a(n) : nth digit,  n ≥ 3 */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* Undefined for n < 3; return 0 as a safe placeholder. */
        return (seq_t)0;
    }

    /* Fraction 149597870700 / 299792458 reduced to 1024642950 / 2053373 */
    const seq_t NUM = (seq_t)1024642950; /* 149597870700 / 146 / 10^? */
    const seq_t DEN = (seq_t)2053373;   /* 299792458 / 146 / 10^? */

    /* Integer part and its digit count */
    seq_t int_part = NUM / DEN;
    idx_t int_digits = 0;
    seq_t tmp = int_part;
    if (tmp == 0) {
        int_digits = 1;
    } else {
        while (tmp > 0) {
            tmp /= (seq_t)10;
            ++int_digits;
        }
    }

    /* Position in the full decimal string (1‑based) */
    idx_t pos = n - 2;     /* because a(3) is the first digit */

    /* Digit from the integer part */
    if (pos <= int_digits) {
        seq_t div = pow10(int_digits - pos);   /* 10^(digits-pos) */
        return (int_part / div) % (seq_t)10;
    }

    /* Digit from the fractional part */
    seq_t rem = NUM % DEN;          /* remainder after integer part */
    idx_t frac_pos = pos - int_digits;  /* how many fractional digits to skip */

    seq_t digit = 0;
    for (idx_t i = 0; i < frac_pos; ++i) {
        rem = rem * (seq_t)10;
        digit = rem / DEN;
        rem = rem % DEN;
    }
    return digit;
}
