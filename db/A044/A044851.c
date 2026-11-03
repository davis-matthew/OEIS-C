/*
 * A044851  Positive integers having no fewer base-10 runs of even length than
 *          odd.
 * https://oeis.org/A044851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer whose decimal representation
   contains at least as many runs of even length as runs of odd length. */

#include <stdbool.h>

/* Determine if a number satisfies the condition: number of even‑length
 * runs >= number of odd‑length runs in its decimal representation. */
static inline bool _has_even_ge_odd(seq_t x)
{
    int even = 0, odd = 0;
    while (x != 0) {
        unsigned int digit = (unsigned int)(x % 10);
        x /= 10;
        int len = 1;
        while (x != 0 && (unsigned int)(x % 10) == digit) {
            len++;
            x /= 10;
        }
        if ((len & 1) == 0)
            even++;
        else
            odd++;
    }
    return even >= odd;
}

/* Return the n-th term of A044851 (1‑based indexing). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 1;
    while (1) {
        if (_has_even_ge_odd(num)) {
            count++;
            if (count == n)
                return num;
        }
        num++;
    }
}
