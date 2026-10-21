/*
 * A069712  Triangular numbers with arithmetic mean of digits = integer (sum
 *          of digits = A multiple of the number of digits).
 * https://oeis.org/A069712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069712: triangular numbers whose digit sum is a multiple of the number of digits.
   a(n) = the (n‑th) triangular T_k = k(k+1)/2 such that
          sum_of_digits(T_k) % number_of_digits(T_k) == 0,
   with a(0) = 1. */

#include <stdint.h>
#include <stdbool.h>

static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

static seq_t digit_len(seq_t x)
{
    seq_t len = 0;
    while (x > 0) {
        ++len;
        x /= 10;
    }
    return len;
}

seq_t a(idx_t n)
{
    idx_t k = 1;          /* index of triangular number T_k */
    idx_t found = 0;      /* number of qualifying terms found */

    while (true) {
        /* Use a wider type for the intermediate to avoid overflow
           when seq_t is small (e.g. uint8_t). */
        uint64_t tri = (uint64_t)k * (k + 1) / 2;
        seq_t T   = (seq_t)tri;

        seq_t sum = digit_sum(T);
        seq_t len = digit_len(T);

        if (len > 0 && (sum % len) == 0) {
            if (found == n)
                return T;
            ++found;
        }
        ++k;
    }
}
