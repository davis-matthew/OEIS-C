/*
 * A085152  All prime factors of n and n+1 are <= 5. (Related to the abc
 *          conjecture.)
 * https://oeis.org/A085152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) are all integers n such that n and n+1 are 5‑smooth (all
 *            prime factors of both n and n+1 are ≤ 5). This finite sequence
 *            contains the terms 1, 2, 3, 4, 5, 8, 9, 15, 24, 80. The function
 *            computes them by scanning integers until the requested term is
 *            found
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   _is5smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_is5smooth.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[10];
    static idx_t count = 0;

    if (!initialized) {
        /* Generate all 10 terms once. */
        idx_t i = 1;
        while (count < 10) {
            seq_t val = (seq_t)i;
            if (_is5smooth(val) && _is5smooth((seq_t)(i + 1))) {
                seq[count++] = val;
            }
            ++i;
        }
        initialized = true;
    }

    if (n >= 1 && n <= count) {
        return seq[n - 1];
    } else {
        /* The sequence terminates after 10 terms. */
        return (seq_t)0;
    }
}
