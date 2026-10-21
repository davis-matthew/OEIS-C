/*
 * A085094  Smallest k such that n*k-1 is a palindrome, or 0 if no such number
 *          exists.
 * https://oeis.org/A085094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that n * k - 1 is a palindrome (base 10)
 * Range:     u8 n <= 76, u16 n <= 120, u32 n <= 120, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: test whether an unsigned 64‑bit integer is a decimal palindrome */
static bool is_palindrome_uint64(idx_t x)
{
    char buf[20];
    size_t len = 0;

    if (x == 0) {
        buf[0] = '0';
        len = 1;
    } else {
        while (x > 0) {
            buf[len++] = '0' + (x % 10);
            x /= 10;
        }
    }

    for (size_t i = 0; i < len / 2; ++i) {
        if (buf[i] != buf[len - 1 - i])
            return false;
    }
    return true;
}

/* The sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1)          /* out of offset bounds: safe guard */
        return 0;

    idx_t k = 1;
    while (1) {
        idx_t prod = n * k - 1;          /* product fits in idx_t for the searches used */
        if (is_palindrome_uint64(prod))
            return (seq_t)k;             /* cast to the build's seq_t type */
        ++k;
    }
}
