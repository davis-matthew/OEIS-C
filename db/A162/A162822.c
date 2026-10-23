/*
 * A162822  Positive numbers k such that 36*k/(36-k) are integers.
 * https://oeis.org/A162822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) are the positive integers k for which 36*k/(36-k) is an
 *            integer. This is equivalent to k = 36 - d where d is a divisor
 *            of 1296 (including d = 0 for the special case k = 36). The
 *            sequence is the sorted list of all such k. Implementation
 *            enumerates all 25 positive divisors of 1296, generates t...
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, only if needed for intermediate types */

seq_t a(idx_t n) {
    /* 25 positive divisors of 1296 = 2^4 * 3^4 */
    static const int pow2[5] = {1, 2, 4, 8, 16};
    static const int pow3[5] = {1, 3, 9, 27, 81};

    static seq_t seq[38];   /* maximal number of terms (12 <36 + 1 + 25 >36) */
    static int seq_len = 0;
    static int initialized = 0;

    if (!initialized) {
        int idx = 0;

        /* generate all positive divisors of 1296 */
        for (int e2 = 0; e2 <= 4; ++e2) {
            for (int e3 = 0; e3 <= 4; ++e3) {
                int d = pow2[e2] * pow3[e3];          /* divisor */
                /* k = 36 + d  (always >0) */
                seq[idx++] = (seq_t)(36 + d);

                /* k = 36 - d  (positive only if d < 36) */
                if (d < 36) {
                    /* d < 36 guarantees k > 0 */
                    seq[idx++] = (seq_t)(36 - d);
                }
            }
        }

        /* add the special k = 36 (d = 0) */
        seq[idx++] = (seq_t)36;

        /* sort the resulting values */
        for (int i = 1; i < idx; ++i) {
            seq_t key = seq[i];
            int j = i - 1;
            while (j >= 0 && seq[j] > key) {
                seq[j + 1] = seq[j];
                --j;
            }
            seq[j + 1] = key;
        }

        seq_len = idx;   /* should be 38 for the given problem */
        initialized = 1;
    }

    if (n == 0 || n > seq_len) {
        return (seq_t)0;   /* undefined for n outside the known range */
    }
    return seq[n - 1];
}
