/*
 * A108114  Numbers n such that the string 3731n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A108114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 946 (2026-11-01)
 * Helpers:   is_prime_seq, num_digits_idx
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

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n) {
    idx_t found = 0;            /* number of sequence elements found   */
    idx_t m = 1;                /* current candidate                   */

    while (1) {
        /* number of decimal digits of m */
        int d = num_digits_idx(m);

        /* compute 3731 * 10^d + m as a seq_t */
        seq_t ten_pow = pow10((idx_t)d);
        seq_t num = (ten_pow * (seq_t)3731) + (seq_t)m;

        if (is_prime_seq(num)) {
            found++;
            if (found == n)
                return (seq_t)m;   /* return the m that makes the concatenation prime */
        }
        m++;
    }
}
