/*
 * A062690  Smallest triangular number that contains the string n in its exact
 *          center.
 * https://oeis.org/A062690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the smallest triangular number T_k = k(k+1)/2 that
 *            contains the decimal string of n in its exact center
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 1028, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_len_dec, seq_to_dec_rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_len_dec.h"
#include "../utils/seq_to_dec_rev.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    /* Convert n to its decimal string (sn), reversed for easy comparison. */
    char sn[32];
    int sn_len = 0;
    idx_t tmpn = n;
    do {
        sn[sn_len++] = (char)('0' + (int)(tmpn % 10));
        tmpn /= 10;
    } while (tmpn > 0);
    /* Reverse sn to normal order. */
    for (int i = 0; i < sn_len / 2; ++i) {
        char c = sn[i];
        sn[i] = sn[sn_len - 1 - i];
        sn[sn_len - 1 - i] = c;
    }

    /* Iterate over triangular numbers until we find a match. */
    for (idx_t k = 1; ; ++k) {
        seq_t N = (seq_t)k * ((seq_t)k + 1) / 2;
        int len = seq_len_dec(N);
        if (len < sn_len) continue;
        if (((len - sn_len) & 1) != 0) continue;   /* center alignment not possible */

        /* Convert N to a character buffer. */
        char buf[128];
        int blen;
        seq_to_dec_rev(N, buf, &blen);
        /* Reverse buf to normal order. */
        for (int i = 0; i < blen / 2; ++i) {
            char c = buf[i];
            buf[i] = buf[blen - 1 - i];
            buf[blen - 1 - i] = c;
        }

        /* Check if the middle substring equals sn. */
        int start = (blen - sn_len) / 2;
        bool ok = true;
        for (int i = 0; i < sn_len; ++i) {
            if (buf[start + i] != sn[i]) { ok = false; break; }
        }
        if (ok) return N;
    }
}
