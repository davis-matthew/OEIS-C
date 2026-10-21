/*
 * A087251  Distinct largest-peak-values in 3x+1 trajectory arranged by their
 *          first appearance as initial value increases. A025586 without
 *          repetitions while keeping order.
 * https://oeis.org/A087251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: distinct new peak values in the 3x+1 collatz trajectory,
 * listed in order of first appearance.  a(1)=1, a(2)=2, a(3)=16,
 * a(4)=4, a(5)=52, …   (offset 1)
 * The implementation keeps a memoised list of peaks and generates
 * further ones on demand.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *peaks = NULL;      /* array of stored peak values       */
    static size_t peaks_len  = 0;    /* how many peaks are stored         */
    static size_t peaks_cap  = 0;    /* current capacity of the array     */
    static idx_t  next_start = 1;    /* smallest starting value not yet processed */

    /* n>=1 is guaranteed by the OEIS offset; the following guard is only
       for defensive programming. */
    if (n == 0) return 0;

    /* Extend the peaks list until it contains at least n entries. */
    while (peaks_len < n) {
        /* Compute the maximum value (peak) reached from start = next_start. */
        seq_t x     = next_start;
        seq_t peak  = x;
        while (x != (seq_t)1) {
            if (x % (seq_t)2 == 0) {
                x = x / (seq_t)2;
            } else {
                x = (seq_t)3 * x + (seq_t)1;
            }
            if (x > peak) {
                peak = x;
            }
        }

        /* Check whether this peak is already in the list. */
        int seen = 0;
        for (size_t i = 0; i < peaks_len; ++i) {
            if (peaks[i] == peak) {
                seen = 1;
                break;
            }
        }

        /* If not seen, store it.  Grow the array if necessary. */
        if (!seen) {
            if (peaks_len == peaks_cap) {
                if (peaks_cap == 0)
                    peaks_cap = 16;
                else
                    peaks_cap <<= 1;
                peaks = (seq_t*)realloc(peaks, peaks_cap * sizeof(seq_t));
            }
            peaks[peaks_len++] = peak;
        }

        /* Move on to the next starting value. */
        ++next_start;
    }

    /* Always keep the nth element ready for any future calls. */
    return peaks[n - 1];
}
