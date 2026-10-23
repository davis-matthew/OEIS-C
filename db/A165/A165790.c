/*
 * A165790  a(n) = the smallest positive multiple, k, of n that when written
 *          in binary, and leading 0's of k are ignored, contains the reversal
 *          of the digits of binary n.
 * https://oeis.org/A165790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive multiple of n whose binary
 *            representation contains the reverse of the binary digits of n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

static void seq_to_bin(seq_t value, char *out, size_t *len)
{
    if (value == (seq_t)0) {
        out[0] = '0';
        *len = 1;
        out[1] = '\0';
        return;
    }
    size_t i = 0;
    seq_t tmp = value;
    while (tmp != (seq_t)0) {
        out[i++] = ((tmp & (seq_t)1) ? '1' : '0');
        tmp >>= 1;
    }
    /* reverse to get MSB … LSB order */
    for (size_t j = 0; j < i / 2; ++j) {
        char t = out[j];
        out[j] = out[i - 1 - j];
        out[i - 1 - j] = t;
    }
    out[i] = '\0';
    *len = i;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined, but avoid crash */

    /* build reverse binary string of n (LSB first) */
    static char revbuf[128];
    size_t rev_len = 0;
    seq_t t = (seq_t)n;
    while (t != (seq_t)0) {
        revbuf[rev_len++] = ((t & (seq_t)1) ? '1' : '0');
        t >>= 1;
    }
    revbuf[rev_len] = '\0';

    /* search for first multiple that contains revbuf */
    static char binbuf[16384];
    for (idx_t m = 1;; ++m) {
        seq_t k = (seq_t)n * (seq_t)m;
        size_t bin_len;
        seq_to_bin(k, binbuf, &bin_len);
        if (strstr(binbuf, revbuf) != NULL)
            return k;
    }
}
