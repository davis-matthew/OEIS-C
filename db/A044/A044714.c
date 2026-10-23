/*
 * A044714  Numbers n such that string 0,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044714: a(n) = n-th positive integer whose decimal representation
 * contains the substring "01" but n+1 does not.  Computed by brute‑force search. */
#include <stdbool.h>
#include <stddef.h>

/* Test whether the decimal representation of x contains the substring "01". */
static bool has01(seq_t x)
{
    if (x == 0) return false;
    seq_t low = x % 10;
    x /= 10;
    while (x > 0) {
        seq_t high = x % 10;
        if (high == 0 && low == 1) return true;
        low = high;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for the sequence */
    seq_t candidate = 1;
    idx_t count = 0;
    for (;;) {
        if (has01(candidate) && !has01(candidate + 1)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
