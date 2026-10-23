/*
 * A103757  Number of contiguous non-i digits in the counting numbers, for
 *          i=0.
 * https://oeis.org/A103757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n): number of contiguous non‑zero digits between successive
 * zeros in the infinite concatenation of natural numbers beginning at 0.
 * The routine simulates the digit stream and counts runs of non‑zero
 * digits.  It returns the length of the n‑th run. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 safely */

    idx_t run_idx = 0;          /* number of runs already finished   */
    seq_t run_len = 0;          /* current run length (non‑zero digits) */

    for (idx_t num = 0; ; ++num) {
        if (num == 0) { /* the single digit “0” */
            if (run_len > 0) {
                ++run_idx;
                if (run_idx == n) return run_len;
            }
            run_len = 0;
            continue;
        }

        /* Extract decimal digits of num into a small array. */
        int digs[20];
        int d = 0;
        uint64_t x = num;
        while (x > 0) {
            digs[d++] = x % 10;
            x /= 10;
        }

        /* Walk the digits from most significant to least. */
        for (int i = d - 1; i >= 0; --i) {
            int dig = digs[i];
            if (dig == 0) {
                if (run_len > 0) {
                    ++run_idx;
                    if (run_idx == n) return run_len;
                }
                run_len = 0;
            } else {
                ++run_len;
            }
        }
    }
}
