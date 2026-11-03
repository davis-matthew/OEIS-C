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
 * Validated: full, pass, 10 terms through n = 10 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x contains no prime factor > 5 */
static bool _is5smooth(seq_t x)
{
    while (x % 2 == 0) x /= 2;
    while (x % 3 == 0) x /= 3;
    while (x % 5 == 0) x /= 5;
    return x == (seq_t)1;
}

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
