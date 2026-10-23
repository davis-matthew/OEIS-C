/*
 * A109133  Numbers k such that (sum of digits)*(number of digits) + 1 is
 *          prime.
 * https://oeis.org/A109133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_digits, digit_sum_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109133: numbers k such that (digit sum of k) * (number of digits of k) + 1 is prime. */
/* No closed formula; we generate terms by scanning integers and testing the condition. */

#include <stdbool.h>
#include "../utils/count_digits.h"
#include "../utils/digit_sum_uint64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find the n-th term (1-indexed) of the sequence. */
    idx_t count = 0;          /* number of terms found so far */
    idx_t candidate = 1;      /* current integer to test */

    while (true) {
        uint64_t cand64 = (uint64_t)candidate;          /* candidate fits 64 bit for expected ranges */
        uint64_t sum = digit_sum_uint64(cand64);        /* sum of decimal digits */
        idx_t len = count_digits(candidate);            /* number of decimal digits */
        uint64_t val = sum * (uint64_t)len + 1;         /* value to test for primality */

        if (is_prime(val)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
