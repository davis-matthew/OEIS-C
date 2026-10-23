/*
 * A174979  Smallest prime p which is a concatenation of n^3 and the cubic
 *          digits 0, 1, 8.
 * https://oeis.org/A174979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   digit_len, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest prime that is a concatenation of n^3
   with the cubic digits 0, 1, 8 on either side.  The algorithm
   iterates over increasing total length of added digits, enumerating
   all combinations of prefixes and suffixes, and returns the first
   prime found.                                       */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/digit_len.h"
#include "../utils/isPrimeGeneric.h"

/* the function mandated by the RE */
seq_t a(idx_t n) {
    /* 1‑based offset; the sequence is defined for n >= 1 */
    if (n == 0) return 0;          /* safety, not needed for valid tests */

    /* cube of n */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    int B = digit_len(cube);       /* number of decimal digits of the cube */

    for (uint64_t L = 0; ; ++L) { /* total number of added digits (prefix+suffix) */

        /* precompute powers of 10 up to B+L */
        size_t powSize = (size_t)(B + L + 1);
        seq_t *pow10 = (seq_t *)malloc(powSize * sizeof(seq_t));
        if (!pow10) return 0;        /* allocation failure – should not happen */
        pow10[0] = 1;
        for (size_t i = 1; i < powSize; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;

        /* 3^k for k = 0 .. L  (to count digit combinations) */
        uint64_t *cnt3 = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
        if (!cnt3) { free(pow10); return 0; }
        cnt3[0] = 1;
        for (uint64_t i = 1; i <= L; ++i)
            cnt3[i] = cnt3[i - 1] * 3ULL;

        seq_t best = 0;
        bool found = false;

        /* enumerate all splits into prefix length p and suffix length s */
        for (uint64_t p = 0; p <= L; ++p) {
            uint64_t s = L - p;
            uint64_t pref_cnt = cnt3[p];
            uint64_t suf_cnt  = cnt3[s];
            seq_t pow10_Bs = pow10[(size_t)(B + s)];
            seq_t pow10_s  = pow10[(size_t)s];

            /* iterate all prefixes */
            for (uint64_t pi = 0; pi < pref_cnt; ++pi) {
                seq_t pref_val = 0;
                uint64_t tmp = pi;
                for (uint64_t i = 0; i < p; ++i) {
                    int d   = tmp % 3;
                    tmp    /= 3;
                    int dig = (d == 0 ? 0 : (d == 1 ? 1 : 8));
                    pref_val = pref_val * 10 + dig;
                }

                /* iterate all suffixes */
                for (uint64_t si = 0; si < suf_cnt; ++si) {
                    seq_t suf_val = 0;
                    uint64_t tmp2 = si;
                    for (uint64_t i = 0; i < s; ++i) {
                        int d   = tmp2 % 3;
                        tmp2    /= 3;
                        int dig = (d == 0 ? 0 : (d == 1 ? 1 : 8));
                        suf_val = suf_val * 10 + dig;
                    }

                    seq_t cand = pref_val * pow10_Bs
                               + cube   * pow10_s
                               + suf_val;
                    if (isPrimeGeneric(cand)) {
                        if (!found || cand < best) {
                            best  = cand;
                            found = true;
                        }
                    }
                }
            }
        }

        free(pow10);
        free(cnt3);

        if (found) return best;   /* smallest prime found for this L */
        /* else continue with greater L */
    }
}
