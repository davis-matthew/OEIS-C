/*
 * A038770  Numbers divisible by at least one of their digits.
 * https://oeis.org/A038770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scan integers until the nth integer is divisible by one of its decimal digits. */
#include <stdbool.h>
#include <stdint.h>

/* Test whether a value has a non‑zero decimal digit that divides it. */
static bool has_dividing_digit(seq_t x)
{
    seq_t t = x;
    while (t > 0) {
        seq_t d = t % 10;   /* current digit */
        t /= 10;
        if (d > 0 && x % d == 0)
            return true;
    }
    return false;
}

/* Return the nth term of OEIS A038770 (1‑based index). */
seq_t a(idx_t n)
{
    uint64_t candidate = 1;
    idx_t count = 0;
    for (;;) {
        if (has_dividing_digit((seq_t)candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
