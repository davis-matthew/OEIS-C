/*
 * A152221  a(n) = the largest integer k where the binary representations of
 *          both k and of k+1 occur as substrings in the binary representation
 *          of n. a(n) = 0 if n = 2^m -1, for m = any positive integer.
 * https://oeis.org/A152221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest integer k where the binary representations of k
 *            and k+1 occur as substrings in the binary representation of n.
 *            a(n)=0 if n = 2^m-1
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* convert unsigned value to binary string (MSB first) */
static void to_bin(uint64_t val, char *buf, int *pLen) {
    int i = 0;
    while (val > 0) {
        buf[i++] = (val & 1) ? '1' : '0';
        val >>= 1;
    }
    /* reverse to MSB first */
    for (int j = 0; j < i / 2; ++j) {
        char t = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    *pLen = i;
}

/* check if pattern pat[0..plen-1] occurs in text txt[0..tlen-1] */
static bool contains(const char *txt, int tlen, const char *pat, int plen) {
    if (plen > tlen) return false;
    for (int i = 0; i <= tlen - plen; ++i) {
        if (txt[i] == pat[0] && memcmp(&txt[i], pat, plen) == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;

    /* if n is 2^m-1 (all bits 1) */
    if ((N & (N + 1)) == 0) return (seq_t)0;

    /* build binary string of N */
    char bits_rev[65];
    int L = 0;
    uint64_t t = N;
    while (t > 0) {
        bits_rev[L++] = (t & 1) ? '1' : '0';
        t >>= 1;
    }
    char bin[65];
    for (int i = 0; i < L; ++i) bin[i] = bits_rev[L - 1 - i];
    bin[L] = '\0';

    uint64_t best = 0;
    char pat_val[65], pat_next[65];
    int len_next;

    /* enumerate all substrings that could be k */
    for (int len = 1; len <= L; ++len) {
        for (int i = 0; i + len <= L; ++i) {
            if (bin[i] != '1') continue;      /* leading 1 required */

            /* convert substring to integer */
            uint64_t val = 0;
            for (int k = 0; k < len; ++k) {
                val = (val << 1) | (bin[i + k] - '0');
            }

            /* binary of val+1 */
            to_bin(val + 1, pat_next, &len_next);
            if (len_next <= L && contains(bin, L, pat_next, len_next)) {
                if (val > best) best = val;
            }
        }
    }

    return (seq_t)best;
}
