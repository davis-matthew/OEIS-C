/*
 * A225607  If the run lengths of the binary representation of n are [1+r_1,
 *          1+r_2, 1+r_3, ..., 1+r_k], then those of a(n) are [1+(r_1), 1+(r_1
 *          XOR r_2), 1+(r_2 XOR r_3), ..., 1+(r_{k-1} XOR r_k)], where XOR
 *          denotes the XOR binary operator.
 * https://oeis.org/A225607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary number obtained from the run‑length
 *            transformation described in the OEIS entry for A225607
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the non‑positive case just in case */
    if (n == 0)
        return 0;

    seq_t res = 0;               /* result being built */
    int cur_bit = 1;             /* MSB of every n >= 1 is 1 */
    int run_len = 0;             /* current run length (in bits) */
    int prev_r = 0;              /* r of previous run, initialized later */
    int first_run = 1;           /* true until the first run ends */

    /* Process bits from most significant to least */
    int high = floor_log2_uint64(n);
    for (int pos = high; pos >= 0; --pos) {
        int bit = (n >> pos) & 1;
        if (bit != cur_bit) {                      /* run ends */
            int r = run_len - 1;                   /* r_i = run_length - 1 */
            unsigned int out_len;                 /* length of run in a(n) */

            if (first_run) {
                out_len = run_len;                /* first run unchanged */
                first_run = 0;
            } else {
                out_len = 1 + (prev_r ^ r);       /* XOR for subsequent runs */
            }

            seq_t pow_len = pow2(out_len);         /* 2^out_len */
            res = res * pow_len;                  /* shift left by out_len */
            if (cur_bit)                          /* add block of ones */
                res = res + (pow_len - 1);

            prev_r = r;                           /* update prev_r */
            cur_bit = bit;                        /* start new run */
            run_len = 1;                          /* first bit of new run */
        } else {
            run_len++;                             /* continue current run */
        }
    }

    /* Final (last) run is still pending */
    int r = run_len - 1;
    unsigned int out_len;
    if (first_run) {    /* n consisted of a single run only */
        out_len = run_len;
    } else {
        out_len = 1 + (prev_r ^ r);
    }

    seq_t pow_len = pow2(out_len);
    res = res * pow_len;
    if (cur_bit)
        res = res + (pow_len - 1);

    return res;
}
