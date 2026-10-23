/*
 * A043704  Numbers whose base-15 representation has an even number of runs.
 * https://oeis.org/A043704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043704: numbers whose base‑15 representation has an even number of runs */
#include <stdbool.h>

/* Return true if x has an even number of runs in base 15 */
static bool even_runs(seq_t x) {
    unsigned char digits[64];
    int pos = 0;

    if (x == 0) {
        digits[pos++] = 0;
    } else {
        seq_t temp = x;
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 15);
            digits[pos++] = digit;
            temp /= 15;
        }
    }

    int runs = 1;
    for (int i = 1; i < pos; ++i) {
        if (digits[i] != digits[i - 1])
            runs++;
    }
    return (runs & 1) == 0;
}

/* a(n): n-th positive integer whose base‑15 representation has an even number of runs */
seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 1;
    for (;;) {
        if (even_runs(k)) {
            ++count;
            if (count == (seq_t)n)
                return k;
        }
        ++k;
    }
}
