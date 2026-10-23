/*
 * A122975  Numbers containing no successive even ternary digits.
 * https://oeis.org/A122975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer whose base‑3 representation
 *            contains no two successive even digits (0 or 2)
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t suffix[65][2];
static seq_t pow2[65];
static int initialized = 0;

seq_t a(idx_t n)
{
    /* Initialise lookup tables once. */
    if (!initialized) {
        pow2[0] = (seq_t)1;
        for (int i = 1; i < 65; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;

        suffix[0][0] = suffix[0][1] = (seq_t)1;
        for (int i = 1; i < 65; ++i) {
            suffix[i][0] = suffix[i-1][1];                       /* next must be odd */
            suffix[i][1] = suffix[i-1][0] * (seq_t)2   /* two evens */
                           + suffix[i-1][1];        /* one odd */
        }
        initialized = 1;
    }

    /* Determine the number of ternary digits L for a(n). */
    idx_t tmp = n + (idx_t)1;
    int bitlen = 0;
    while (tmp > 0) {
        ++bitlen;
        tmp >>= 1;
    }
    int L = bitlen - 1;                /* L ≥ 1 for n ≥ 1 */

    /* offset within the block of L‑digit numbers (1‑based). */
    seq_t offset = (seq_t)n - (pow2[L] - (seq_t)2);

    /* Unrank the offset‐th valid ternary string of length L. */
    seq_t val = (seq_t)0;
    int prev_parity = -1;              /* parity of previous digit: 0 even, 1 odd */

    for (int pos = 1; pos <= L; ++pos) {
        int remaining = L - pos;
        for (int d = 0; d <= 2; ++d) {
            if (pos == 1 && d == 0)          /* leading digit cannot be zero */
                continue;
            if (pos > 1 && prev_parity == 0 && d != 1) /* even followed by even */
                continue;

            seq_t cnt = suffix[remaining][d & 1];
            if (offset > cnt) {
                offset -= cnt;
                continue;
            }

            val = val * (seq_t)3 + (seq_t)d;
            prev_parity = d & 1;
            break;
        }
    }
    return val;
}
