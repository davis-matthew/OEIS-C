/*
 * A101933  Concatenate the first n positive integers written in binary (with
 *          each integer written so the most significant 1 is on the left and
 *          the concatenated string is from left to right, A030302). a(n) is
 *          the number of times n written in binary appears disjointedly in
 *          the concatenated string.
 * https://oeis.org/A101933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of disjoint occurrences of the binary
 *            representation of n in the concatenation of the binary
 *            representations of 1,2,…,n. The sequence offset is 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper: number of bits needed to represent x (x>0).  */
static inline idx_t bitlen(idx_t x)
{
    idx_t len = 0;
    do {
        ++len;
        x >>= 1;
    } while (x);
    return len;
}

/* External linkage function as required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset guard */

    /* --- build the binary pattern for n --- */
    idx_t patlen = bitlen(n);       /* length of n in binary */
    char *pat = (char *)malloc((size_t)patlen);
    if (!pat) return 0;

    for (idx_t i = 0; i < patlen; ++i)
        pat[i] = ((n >> (patlen - 1 - i)) & 1) ? '1' : '0';

    /* --- total number of bits in the concatenated string up to n --- */
    idx_t total_bits = 0;
    for (idx_t k = 1; k <= n; ++k)
        total_bits += bitlen(k);

    /* --- build the concatenated bit-string --- */
    char *buf = (char *)malloc((size_t)total_bits);
    if (!buf) {
        free(pat);
        return 0;
    }

    idx_t pos = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t len = bitlen(k);
        for (idx_t j = 0; j < len; ++j)
            buf[pos++] = ((k >> (len - 1 - j)) & 1) ? '1' : '0';
    }

    /* --- count non‑overlapping occurrences of pat in buf --- */
    seq_t count = 0;
    for (idx_t i = 0; i + patlen <= total_bits; ) {
        if (memcmp(buf + i, pat, patlen) == 0) {
            ++count;
            i += patlen;           /* skip to avoid overlap */
        } else {
            ++i;
        }
    }

    free(pat);
    free(buf);
    return count;
}
