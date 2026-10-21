/*
 * A224953  Number of ways a digit can be appended or prepended to n and form
 *          a prime.
 * https://oeis.org/A224953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of primes obtained by concatenating a single
 *            decimal digit either before or after n. The function tests all
 *            ten possible prepended digits and all ten appended digits and
 *            counts how many of the resulting numbers are prime. It uses the
 *            helper function is_prime_big which handles arbitr...
 * Range:     u8 n <= 10, u16 n <= 1054, u32 n <= 4886, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute decimal length of n */
    uint64_t len = 0;
    uint64_t tmp = n;
    if (tmp == 0) {
        len = 1;
    } else {
        while (tmp) {
            ++len;
            tmp /= 10;
        }
    }

    /* 10^len as seq_t */
    seq_t pow10_val = pow10((idx_t)len);

    seq_t count = 0;

    for (uint64_t d = 0; d < 10; ++d) {
        /* prepend digit d */
        seq_t val_prepend = (seq_t)d * pow10_val + (seq_t)n;
        if (is_prime_big(val_prepend)) {
            ++count;
        }

        /* append digit d */
        seq_t val_append = (seq_t)n * 10 + (seq_t)d;
        if (is_prime_big(val_append)) {
            ++count;
        }
    }

    return count;
}
