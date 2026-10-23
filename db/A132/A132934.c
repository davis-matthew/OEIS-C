/*
 * A132934  Concatenation of first n nonprime numbers.
 * https://oeis.org/A132934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   digits_of, is_prime, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of the first n nonprime numbers.
   a(1) = 1, a(2) = 14, a(3) = 146, … */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/digits_of.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; safe fallback */
    seq_t res   = 0;
    idx_t count = 0;
    idx_t x     = 1;

    while (count < n) {
        if (!is_prime(x)) {        /* x is not prime (includes 1) */
            int d = digits_of(x); /* decimal digit count of x */
            res = seq_mul(res, pow10(d)) + x;
            ++count;
        }
        ++x;
    }
    return res;
}
