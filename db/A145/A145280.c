/*
 * A145280  Fecundity of n-th prime.
 * https://oeis.org/A145280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A070562(prime(n)) where A070562(x) is the number of
 *            iterations of x += product_of_digits(x) needed to reach a number
 *            containing a zero digit. For an n-th prime p, a(n) = number of
 *            such iterations starting from p
 * Range:     u8 n <= 32, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digit_product, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digit_product.h"

/* Check if a number contains a zero digit */
static inline bool has_zero_digit(seq_t v)
{
    while (v > 0) {
        if ((v % (seq_t)10) == 0)
            return true;
        v /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    uint32_t p = get_prime(n);

    seq_t cur = (seq_t)p;
    int steps = 0;

    while (!has_zero_digit(cur)) {
        /* Add the product of the digits to the current value. */
        cur += digit_product(cur);
        steps++;
    }

    return (seq_t)steps;
}
