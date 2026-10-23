/*
 * A181525  Prime numbers such that the difference between any pair of digits
 *          is 0, 2, 3, 5 or 7.
 * https://oeis.org/A181525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by a brute‑force search:
   a(n) is the n-th prime whose every pair of decimal digits differs
   by 0, 2, 3, 5 or 7. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

/* Helper: test whether a prime satisfies the digit difference condition. */
static bool digit_diff_valid(seq_t p)
{
    /* buffer large enough for the expected digit count */
    uint8_t digits[8192];
    size_t len = 0;
    get_digits(p, digits, &len);

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            int d = (int)digits[i] - (int)digits[j];
            if (d < 0) d = -d;
            if (d != 0 && d != 2 && d != 3 && d != 5 && d != 7)
                return false;
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1 – return 0 */
        return 0;

    /* Handle the first prime specially. */
    if (n == 1)
        return (seq_t)2;

    /* Count of valid primes found so far. */
    idx_t found = 1;          /* prime 2 is already counted */

    /* Start checking with the next odd number. */
    seq_t candidate = 3;

    while (found < n) {
        if (is_prime_big(candidate) && digit_diff_valid(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        candidate += 2;  /* skip even numbers */
    }

    /* Unreachable – the loop always returns. */
    return (seq_t)0;
}
