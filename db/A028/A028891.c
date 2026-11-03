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
 * Validated: standard, pass, 80 terms through n = 959 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if the iterated product of the digits of x is a Fibonacci
   number.  The Fibonacci numbers we encounter after iterating are
   1, 2, 3, 5, 8 (single digits). */
static bool is_fib_digit_product(uint64_t x)
{
    /* First product of digits, abort if a zero digit is found. */
    uint64_t prod = 1;
    for (uint64_t t = x; t > 0; t /= 10) {
        uint64_t d = t % 10;
        if (d == 0) return false;            /* product becomes 0 → not Fibonacci */
        prod *= d;
    }

    /* Continue iterating while the product has two or more digits. */
    while (prod >= 10) {
        uint64_t next = 1;
        for (uint64_t t = prod; t > 0; t /= 10) {
            next *= t % 10;
        }
        prod = next;
    }

    /* prod is now a single digit.  Check against the Fibonacci set. */
    return (prod == 1 || prod == 2 || prod == 3 || prod == 5 || prod == 8);
}

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
