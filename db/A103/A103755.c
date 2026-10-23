/*
 * A103755  Number of contiguous digits i in the counting numbers, for i=1.
 * https://oeis.org/A103755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th group length of consecutive '1' digits in the
   infinite concatenation of decimal representations of natural
   numbers.  It scans the digits sequentially, keeping a running
   count of consecutive '1's; when a non‑1 digit terminates a run,
   the run count is recorded.  When the index of the completed run
   equals n, the length is returned. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t current_run = 0;   /* length of the current run of '1's */
    idx_t run_index = 0;     /* number of completed runs seen so far */
    idx_t num = 1;           /* current natural number to process */

    while (1) {
        uint64_t t = (uint64_t)num;
        /* Extract the decimal digits of t into an array in reverse order. */
        int digits[20];
        int len = 0;
        while (t > 0) {
            digits[len++] = (int)(t % 10);
            t /= 10;
        }

        /* Process the digits from most significant to least significant. */
        for (int i = len - 1; i >= 0; --i) {
            int d = digits[i];
            if (d == 1) {
                current_run = current_run ? current_run + 1 : 1;
            } else {
                if (current_run) {
                    ++run_index;          /* run has ended */
                    if (run_index == n)   /* we found the nth run */
                        return current_run;
                    current_run = 0;
                }
            }
        }

        ++num;   /* move to the next natural number */
    }
}
