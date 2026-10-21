/*
 * A087217  In decimal representation: smallest multiple of n containing it as
 *          substring.
 * https://oeis.org/A087217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer > n that is a multiple of n and whose
 *            decimal representation contains the decimal representation of n.
 *            Equivalently, a(n) = n * A086064(n). This implementation
 *            searches for a multiplier m from 2 to 10 (m = 10 always
 *            succeeds: n*10 = n followed by a zero)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t */
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;

    /* digits of n in most‑significant‑first order */
    uint8_t nbuf[64];
    size_t nlen;
    get_digits(seq_n, nbuf, &nlen);

    /* try multipliers from 2 to 10 inclusive */
    for (idx_t m = 2; m <= 10; ++m) {
        seq_t prod = seq_n * (seq_t)m;
        uint8_t pb[64];
        size_t plen;
        get_digits(prod, pb, &plen);

        if (plen < nlen) continue;

        /* check for n as a contiguous substring of prod */
        for (size_t i = 0; i + nlen <= plen; ++i) {
            size_t j = 0;
            for (; j < nlen; ++j) {
                if (pb[i + j] != nbuf[j]) break;
            }
            if (j == nlen) return prod;
        }
    }

    /* m=10 is guaranteed to succeed; this line is only for completeness. */
    return seq_n * ((seq_t)10);
}
