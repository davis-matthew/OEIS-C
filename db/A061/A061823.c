/*
 * A061823  Multiples of 7 containing only digits 0,...,7.
 * https://oeis.org/A061823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3928, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A061823: multiples of 7 whose decimal
   digits are all ≤ 7.  The sequence is 0,7,14,21,35,42,... */

#include <stdbool.h>

static bool allowed_digits(seq_t x)
{
    /* By definition 0 qualifies; otherwise check all decimal digits. */
    if (x == 0) return true;
    while (x > 0) {
        uint64_t d = (uint64_t)(x % 10);
        if (d > 7) return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Empty offset handling: for n < 1, simply return 0. */
    if (n == 0) return 0;

    idx_t count = 0;
    seq_t m = 0;                    /* start with 0, the first term */
    const seq_t seven = (seq_t)7;

    for (;;) {
        if (allowed_digits(m)) {
            ++count;
            if (count == n) return m;
        }
        m += seven;
    }
}
