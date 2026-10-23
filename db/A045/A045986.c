/*
 * A045986  Describe the n-th triangular number A000217(n).
 * https://oeis.org/A045986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   for each run of equal consecutive decimal digits of the n‑th
 *            triangular number T(n)=n*(n+1)/2, output the run length followed
 *            by the digit. The result is the concatenation of these
 *            run‑length/digit pairs
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Compute T(n) as a seq_t value */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Extract decimal digits (least significant first) */
    unsigned digits[64];
    size_t cnt = 0;
    if (tn == 0) {
        digits[cnt++] = 0;
    } else {
        seq_t temp = tn;
        while (temp > 0) {
            seq_t rem = temp % 10;
            digits[cnt++] = (unsigned)rem;   /* rem fits in 0..9 */
            temp /= 10;
        }
    }

    /* Build the output string */
    char out[128];
    size_t outlen = 0;
    size_t i = cnt;
    while (i > 0) {
        i--;
        unsigned d = digits[i];  /* current digit */
        unsigned run = 1;
        while (i > 0 && digits[i-1] == d) {
            i--;
            run++;
        }
        /* Output run length */
        unsigned tmp[32], tmplen = 0;
        if (run == 0) {
            tmp[tmplen++] = 0;
        } else {
            unsigned r = run;
            while (r > 0) {
                tmp[tmplen++] = r % 10;
                r /= 10;
            }
            /* Reverse to correct order */
            for (size_t a = 0; a < tmplen / 2; ++a) {
                unsigned x = tmp[a];
                tmp[a] = tmp[tmplen - 1 - a];
                tmp[tmplen - 1 - a] = x;
            }
        }
        for (size_t a = 0; a < tmplen; ++a) {
            out[outlen++] = '0' + tmp[a];
        }
        /* Output the digit itself */
        out[outlen++] = '0' + d;
    }
    out[outlen] = '\0';

    /* Convert the concatenated string back to seq_t */
    return seq_from_str(out);
}
