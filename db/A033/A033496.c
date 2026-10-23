/*
 * A033496  Numbers m that are the largest number in their Collatz (3x+1)
 *          trajectory.
 * https://oeis.org/A033496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Peaks of the Collatz (3x+1) trajectory:
 * a(n) is the nth integer m (n ≥ 1) such that the Collatz
 * trajectory started at m never reaches a value larger than m.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether m is a peak */
static bool is_peak(seq_t m)
{
    seq_t v = m;
    while (1) {
        if (v < (seq_t)2)          /* reached 1 (or 0) ⇒ peak confirmed */
            return true;
        if ((v & (seq_t)1) == 0)   /* even */
            v = v >> 1;
        else {                     /* odd */
            /* 3*v+1 is guaranteed to be even, but we check only the value */
            seq_t nv = v * (seq_t)3 + (seq_t)1;
            if (nv > m)            /* exceeded starting m: not a peak */
                return false;
            v = nv;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of discovered peaks */
    static seq_t *peaks = NULL;
    static size_t peak_cap = 0;
    static size_t peak_len = 0;
    static idx_t next_m = 1;            /* next candidate to test */

    /* Ensure cache capacity */
    if (peak_cap == 0) {
        peak_cap = 64;
        peaks = (seq_t *)malloc(peak_cap * sizeof(seq_t));
    }

    /* Generate peaks until we have enough for the requested n */
    while (peak_len < n) {
        if (is_peak((seq_t)next_m))
            peaks[peak_len++] = (seq_t)next_m;

        /* Expand cache if necessary */
        if (peak_len == peak_cap) {
            peak_cap <<= 1;
            peaks = (seq_t *)realloc(peaks, peak_cap * sizeof(seq_t));
        }
        ++next_m;
    }

    /* Return the nth peak (1‑based indexing) */
    return peaks[n - 1];
}
