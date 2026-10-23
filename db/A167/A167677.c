/*
 * A167677  Replace prime digits with 1 and nonprime digits with 2.
 * https://oeis.org/A167677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A167677
   a(n) = replace each decimal digit of n: prime digits -> 1, nonprime digits -> 2
   (prime digits: 2,3,5,7; nonprime digits: 0,1,4,6,8,9) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)2;

    /* Mapping from digit to transformed digit */
    static const bool prime_digit[10] = {
        false, /* 0 */
        false, /* 1 */
        true,  /* 2 */
        true,  /* 3 */
        false, /* 4 */
        true,  /* 5 */
        false, /* 6 */
        true,  /* 7 */
        false, /* 8 */
        false  /* 9 */
    };

    seq_t result = 0;
    seq_t mult   = 1;

    while (n > 0) {
        idx_t d = n % 10;      /* current decimal digit */
        n   = n / 10;

        seq_t td = prime_digit[d] ? (seq_t)1 : (seq_t)2;
        result += td * mult;
        mult *= (seq_t)10;
    }

    return result;
}
