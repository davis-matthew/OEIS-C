/*
 * A031499  Numbers whose base-10 representation has one fewer 0 than 9's.
 * https://oeis.org/A031499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: numbers whose decimal representation has one fewer 0 than 9's.
   The function finds the n-th such number in increasing order. */
#include <stdbool.h>

static inline bool good(seq_t v)
{
    unsigned zeros = 0, nines = 0;
    if (v == 0) return false;
    while (v > 0) {
        unsigned d = (unsigned)(v % (seq_t)10);
        if (d == 0) zeros++;
        else if (d == 9) nines++;
        v /= (seq_t)10;
    }
    return nines > 0 && zeros == nines - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, but safe */
    seq_t candidate = (seq_t)9;
    idx_t count = 0;

    while (true) {
        if (good(candidate)) {
            if (++count == n) return candidate;
        }
        candidate++;
    }
}
