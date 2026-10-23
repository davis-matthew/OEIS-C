/*
 * A185107  a(n) is the first digit of prime(n) minus the sum of the other
 *          digits.
 * https://oeis.org/A185107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, less
 * Formula:   a(n) = first digit of prime(n) minus (sum of the other digits)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Find the n‑th prime by simple iteration with the supplied is_prime */
static uint64_t nth_prime(idx_t n)
{
    uint64_t candidate = 2;
    idx_t count = 0;
    while (1)
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}

/* Compute the desired value */
seq_t a(idx_t n)
{
    /* 1. Obtain the n‑th prime */
    uint64_t p = nth_prime(n);

    /* 2. Extract the most significant digit */
    uint64_t first = p;
    while (first >= 10u)
        first /= 10u;

    /* 3. Compute the digit sum */
    seq_t sum = 0;
    uint64_t tmp = p;
    while (tmp > 0u)
    {
        sum += (seq_t)(tmp % 10u);
        tmp /= 10u;
    }

    /* 4. Result = first – (sum – first) = 2*first – sum */
    seq_t result = (seq_t)first * 2 - sum;
    return result;
}
