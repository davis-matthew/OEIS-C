/*
 * A023097  a(n) is least k such that k and 5k are anagrams in base n (written
 *          in base 10).
 * https://oeis.org/A023097
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that k and 5k are anagrams when written in
 *            base n (digits 0…n‑1)
 * Range:     u8 none, u16 n <= 9, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* compute gcd for 64 bit values */
static uint64_t _gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* The sequence is defined only for n >= 6; for smaller n the value is unused */
    if (n < 6) return 0;

    uint64_t nn = (uint64_t)n;                     /* use plain 64‑bit for temporary calculations            */
    uint64_t n1 = nn - 1;
    uint64_t g  = _gcd_u64(4, n1);
    uint64_t d  = n1 / g;                          /* k must be a multiple of this value                    */

    /* buffers for digit frequencies (maximum base considered is 10000) */
    static unsigned short cnt1[10010];
    static unsigned short cnt2[10010];

    seq_t base = (seq_t)n;                         /* base as seq_t for arithmetic with seq_t values        */

    for (uint64_t t = 1;; ++t) {
        seq_t k = (seq_t)d * (seq_t)t;             /* candidate k                                         */

        /* count digits of k in base n */
        memset(cnt1, 0, (size_t)nn * sizeof(unsigned short));
        seq_t tmp = k;
        while (tmp > 0) {
            unsigned dgt = (unsigned)(tmp % base);
            ++cnt1[dgt];
            tmp /= base;
        }

        /* count digits of 5k in base n */
        memset(cnt2, 0, (size_t)nn * sizeof(unsigned short));
        tmp = (seq_t)5 * k;
        while (tmp > 0) {
            unsigned dgt = (unsigned)(tmp % base);
            ++cnt2[dgt];
            tmp /= base;
        }

        /* compare the two frequency tables */
        bool ok = true;
        for (uint64_t i = 0; i < nn; ++i) {
            if (cnt1[i] != cnt2[i]) { ok = false; break; }
        }
        if (ok) return k;                          /* found the least such k                               */
    }
}
