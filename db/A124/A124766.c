/*
 * A124766  Number of monotonically increasing runs for compositions in
 *          standard order.
 * https://oeis.org/A124766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(n) = number of monotonically increasing runs in the
 * composition of index n in the “standard order” of all compositions. */
/* The “standard order” of a composition is given by the algorithm
 * described in OEIS sequence A066099: for index k (>0) let
 *   b = k - 2^(floor(log2(k))+ - 1)
 * the bits of b (m-1 bits where m = floor(log2(k))+1) indicate separators
 * between parts.  The resulting list of part sizes is then used to count the
 * number of runs, where a new run starts whenever the next part is smaller
 * than the previous part.  For n = 0, a(0) = 0. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Determine m such that 2^(m-1) <= n < 2^m   (m = ceil(log2(n+1)) ) */
    idx_t floor_log = 63 - __builtin_clzll(n);    /* floor(log2(n)) */
    idx_t m = floor_log + 1;                      /* m = ceil(log2(n+1)) */

    idx_t base = (idx_t)1 << (m - 1);            /* 2^(m-1) */
    idx_t b = n - base;                          /* offset within block */

    /* Run counting */
    seq_t run_count = 0;
    seq_t prev_part = 0;
    seq_t curr_len  = 1;                         /* first element counted */

    for (idx_t i = 1; i < (idx_t)m; ++i) {
        /* Check if there is a separator after element i.
           Separator after element i corresponds to bit at position (m-1-i). */
        if ((b >> (m - 1 - i)) & (idx_t)1) {
            /* Finalize current part */
            seq_t part = curr_len;
            if (run_count == 0) {
                /* First part */
                run_count = 1;
            } else if (part < prev_part) {
                /* New run starts */
                ++run_count;
            }
            prev_part = part;
            curr_len = 1;   /* start next part with next element */
        } else {
            /* Extend current part with the next element */
            ++curr_len;
        }
    }

    /* Finalize the last part (including the last element) */
    seq_t part = curr_len;
    if (run_count == 0) {
        run_count = 1;
    } else if (part < prev_part) {
        ++run_count;
    }
    /* prev_part = part; */ /* not needed further */

    return run_count;
}
