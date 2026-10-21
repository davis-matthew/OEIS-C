/*
 * A056062  Run lengths in infinite binary Champernowne string (A030190)
 *          formed by concatenation of 0, 1, 2 ... in base 2.
 * https://oeis.org/A056062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, base
 * Formula:   a(n) = length of the n-th run of equal bits in the infinite
 *            binary Champernowne string (concatenation of 0, 1, 2, … in base
 *            2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the index of the most significant set bit of n.
   Floor of log2(n). For n == 0 the result is 0. */
static inline idx_t msb_idx(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1u)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t target  = n;     /* run index to return */
    idx_t run_idx = 0;     /* current run index */
    seq_t run_len = 0;     /* length of the current run */
    int   run_bit = -1;    /* bit value of the current run */

    for (idx_t k = 0;; ++k) {
        idx_t msb = msb_idx(k);            /* highest bit position of k */
        for (int pos = (int)msb; pos >= 0; --pos) {
            int bit = (int)((k >> pos) & 1u);

            if (run_len == 0) {            /* first bit of the very first run */
                run_bit = bit;
                run_len = 1;
            } else if (bit == run_bit) {
                ++run_len;                 /* extend current run */
            } else {
                /* run finished – check if it was the requested one */
                if (run_idx == target)
                    return run_len;
                ++run_idx;                  /* move to next run */
                run_bit = bit;
                run_len = 1;
            }
        }
    }
}
