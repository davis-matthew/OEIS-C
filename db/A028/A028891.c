/*
 * A028891  Iterated product of digits of n is a positive Fibonacci number.
 * https://oeis.org/A028891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose iterated product of
 *            digits eventually becomes a positive Fibonacci number
 *            (1,2,3,5,8)
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_fib_digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_fib_digit_product.h"

/* OEIS sequence A028891 */
seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t candidate = 1;

    while (1) {
        if (is_fib_digit_product(candidate)) {
            if (count == n) {
                return (seq_t)candidate;
            }
            ++count;
        }
        ++candidate;
    }
}
