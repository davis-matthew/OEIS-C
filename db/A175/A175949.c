/*
 * A175949  Numbers obtained by concatenation of the binary representation of
 *          A175946(n) and A175945(n).
 * https://oeis.org/A175949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = value of binary string obtained by interpreting the
 *            decimal * concatenation of the run‑length encoding of the runs
 *            of 0s followed by the * run‑length encoding of the runs of 1s in
 *            the binary representation of n * as a run‑length code starting
 *            with bit 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   add_run_to_dec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/add_run_to_dec.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is defined for n ≥ 1 */

    /* Determine bit length of n */
    int bitlen = 0;
    idx_t temp = n;
    while (temp) {
        temp >>= 1;
        ++bitlen;
    }

    /* Extract bits from most significant to least significant */
    int bits[64];
    for (int i = 0; i < bitlen; ++i) {
        idx_t shift = bitlen - 1 - i;
        bits[i] = (int)((n >> shift) & 1ULL);
    }

    /* Run‑length encoding */
    int zero_runs[64], one_runs[64];
    int zcnt = 0, ocnt = 0;

    int cur_bit = bits[0];
    int run_len = 1;
    for (int i = 1; i < bitlen; ++i) {
        if (bits[i] == cur_bit) {
            ++run_len;
        } else {
            if (cur_bit == 0)
                zero_runs[zcnt++] = run_len;
            else
                one_runs[ocnt++] = run_len;
            cur_bit = bits[i];
            run_len = 1;
        }
    }
    /* final run */
    if (cur_bit == 0)
        zero_runs[zcnt++] = run_len;
    else
        one_runs[ocnt++] = run_len;

    /* Build decimal string of concatenated run lengths */
    unsigned char dec[256];
    int dec_len = 0;
    for (int i = 0; i < zcnt; ++i)
        add_run_to_dec(zero_runs[i], dec, &dec_len);
    for (int i = 0; i < ocnt; ++i)
        add_run_to_dec(one_runs[i], dec, &dec_len);

    /* Interpret decimal digits as run‑length code starting with bit 1 */
    seq_t val = 0;
    int cur = 1;
    for (int i = 0; i < dec_len; ++i) {
        int d = dec[i];
        for (int j = 0; j < d; ++j) {
            val = (val << 1) | (seq_t)cur;
        }
        cur ^= 1;
    }

    return val;
}
