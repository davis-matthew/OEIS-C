/*
 * A214957  Numbers for which the sum of reciprocals of nonzero digits is an
 *          integer.
 * https://oeis.org/A214957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers for which the sum of reciprocals of nonzero digits is an
 *            integer. Computed by brute‑force search
 * Range:     u8 n <= 16, u16 n <= 902, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned int invval[10] = {
    0,   // 0
    2520,     // 1/1
    1260,     // 1/2
    840,      // 1/3
    630,      // 1/4
    504,      // 1/5
    420,      // 1/6
    360,      // 1/7
    315,      // 1/8
    280       // 1/9
};

/* Returns true iff the integer x has an integer sum of reciprocals of its non‑zero digits. */
static int is_valid(idx_t x)
{
    if (x == 0) return 1;   /* 0 has an empty sum, which is 0 (an integer). */
    unsigned int sum = 0;
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d) sum += invval[d];
        x /= 10;
    }
    return (sum % 2520) == 0;
}

seq_t a(idx_t n)
{
    /* The sequence is indexed starting at 1. */
    if (n == 0) return 0;  /* Undefined for n == 0, but return 0 for safety. */

    idx_t count = 0;
    idx_t val   = 0;

    while (count < n) {
        if (is_valid(val)) {
            ++count;
            if (count == n) {
                return (seq_t)val;
            }
        }
        ++val;
    }
    /* Never reaches here. */
    return 0;
}
