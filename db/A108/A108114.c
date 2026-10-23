/*
 * A108114  Numbers n such that the string 3731n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A108114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   count_digits, isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula: a(n) is the n-th positive integer m such that the decimal
 *  concatenation "3731" ++ decimal(m) is a prime number.
 *
 *  Implementation:  iterate over m = 1, 2, 3, …, and test primality of
 *  3731 * 10^d + m where d = number of decimal digits of m.
 *
 *  Each candidate is checked with isprime_big(seq_t), which handles
 *  both 64‑bit and arbitrary‑precision integers.
 */

#include "../utils/isprime_big.h"
#include "../utils/pow10.h"
#include "../utils/count_digits.h"

seq_t a(idx_t n) {
    idx_t found = 0;            /* number of sequence elements found   */
    idx_t m = 1;                /* current candidate                   */

    while (1) {
        /* number of decimal digits of m */
        int d = count_digits(m);

        /* compute 3731 * 10^d + m as a seq_t */
        seq_t ten_pow = pow10((idx_t)d);
        seq_t num = (ten_pow * (seq_t)3731) + (seq_t)m;

        if (isprime_big(num)) {
            found++;
            if (found == n)
                return (seq_t)m;   /* return the m that makes the concatenation prime */
        }
        m++;
    }
}
