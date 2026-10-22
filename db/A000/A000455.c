/*
 * A000455  Digits of powers of 2.
 * https://oeis.org/A000455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th digit of the concatenation 1 2 4 8 16 32 64 128 ...
 *            of the decimal powers of 2 (computed by repeated doubling of a
 *            decimal digit array)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned char *a455_seq = NULL;   /* concatenated digits */
static size_t a455_len = 0, a455_cap = 0;
static unsigned char *a455_pow = NULL;   /* current power of 2, little-endian digits */
static size_t a455_plen = 0, a455_pcap = 0;

static void a455_extend(size_t need) {
    if (a455_pow == NULL) {
        a455_pcap = 16;
        a455_pow = (unsigned char *)malloc(a455_pcap);
        a455_pow[0] = 1;
        a455_plen = 1;
    }
    while (a455_len < need) {
        size_t i;
        /* append current power, most significant digit first */
        if (a455_len + a455_plen > a455_cap) {
            size_t nc = a455_cap ? a455_cap : 64;
            while (nc < a455_len + a455_plen) nc *= 2;
            a455_seq = (unsigned char *)realloc(a455_seq, nc);
            a455_cap = nc;
        }
        for (i = 0; i < a455_plen; i++)
            a455_seq[a455_len + i] = a455_pow[a455_plen - 1 - i];
        a455_len += a455_plen;
        /* double the power */
        {
            unsigned carry = 0;
            for (i = 0; i < a455_plen; i++) {
                unsigned v = (unsigned)a455_pow[i] * 2u + carry;
                a455_pow[i] = (unsigned char)(v % 10u);
                carry = v / 10u;
            }
            if (carry) {
                if (a455_plen + 1 > a455_pcap) {
                    a455_pcap *= 2;
                    a455_pow = (unsigned char *)realloc(a455_pow, a455_pcap);
                }
                a455_pow[a455_plen++] = (unsigned char)carry;
            }
        }
    }
}

seq_t a(idx_t n) {
    a455_extend((size_t)n + 1);
    return (seq_t)a455_seq[(size_t)n];
}
