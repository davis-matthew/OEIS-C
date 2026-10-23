/*
 * A046431  Sum of digits of a(n) raised to its digits powers is prime.
 * https://oeis.org/A046431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   big_pow, get_digits, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes terms of OEIS A046431:
   a(n) is the n-th integer x ≥ 10 such that
   Σ(d in digits(x)) d^d is prime.  The implementation
   finds each term by brute force searching from the
   lowest candidate, without any caching. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/big_pow.h"
#include "../utils/isPrimeGeneric.h"

/* Maximum number of decimal digits expected to be seen while searching.
   256 digits is far more than needed for n ≤ 10 000. */
#define MAX_DIGITS 256

static seq_t digit_power_sum(seq_t x)
{
    uint8_t digits[MAX_DIGITS];
    size_t dlen = 0;
    get_digits(x, digits, &dlen);

    seq_t sum = 0;
    for (size_t i = 0; i < dlen; ++i) {
        seq_t d = (seq_t)digits[i];
        sum += big_pow(d, d);          /* d^d */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Search starting from the first admissible integer (10). */
    seq_t candidate = (seq_t)10;
    idx_t found = 0;                 /* number of terms found so far */

    for (;;) {
        seq_t s = digit_power_sum(candidate);

        if (isPrimeGeneric(s)) {
            if (found == n) {
                return candidate;
            }
            ++found;
        }
        candidate += (seq_t)1;       /* move to next integer */
    }
}
