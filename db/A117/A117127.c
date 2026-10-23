/*
 * A117127  Concatenate the first n positive integers written in binary (with
 *          each integer written so the most significant 1 is on the left and
 *          the concatenated string is from left to right). a(n) is the number
 *          of times n written in binary appears in the concatenated string.
 * https://oeis.org/A117127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: build the concatenation of the binary representations of
   1..n, then count overlapping occurrences of the binary representation
   of n.  The resulting count is a(n). No closed‑form formula is known. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset; for n <= 0 the sequence is undefined */
    if (n == 0) return 0;

    /* --- 1. compute total length of the concatenated bit string  --- */
    size_t total_bits = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int len = 0;
        idx_t tmp = k;
        while (tmp) { len++; tmp >>= 1; }
        total_bits += (size_t)len;
    }

    /* --- 2. build the concatenated string S(n)  --- */
    char *buf = (char *)malloc(total_bits);
    if (!buf) return 0;          /* allocation failure */
    size_t pos = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int len = 0;
        idx_t tmp = k;
        while (tmp) { len++; tmp >>= 1; }
        for (int i = len - 1; i >= 0; --i) {
            buf[pos++] = '0' + (char)((k >> i) & 1ULL);
        }
    }

    /* --- 3. produce the pattern P(n) --- */
    int L = 0;
    idx_t tmp = n;
    while (tmp) { L++; tmp >>= 1; }
    char *pat = (char *)malloc((size_t)L);
    if (!pat) { free(buf); return 0; }
    for (int i = 0; i < L; ++i) {
        pat[i] = '0' + (char)((n >> (L - 1 - i)) & 1ULL);
    }

    /* --- 4. count overlapping occurrences of P(n) in S(n) --- */
    seq_t count = 0;
    for (size_t i = 0; i + L <= total_bits; ++i) {
        size_t j = 0;
        for (; j < (size_t)L; ++j) {
            if (buf[i + j] != pat[j]) break;
        }
        if (j == (size_t)L) ++count;
    }

    free(buf);
    free(pat);
    return count;
}
