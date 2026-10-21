/*
 * A098162  a(n+1) = smallest number greater than a(n) having with a(n) a
 *          common divisor which is used before as such a common divisor at
 *          most once; a(1) = 1.
 * https://oeis.org/A098162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = smallest k > a(n‑1) such that
 *            gcd(a(n‑1), k) has appeared at most once before as a gcd of a
 *            pair of consecutive terms
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static seq_t *divs = NULL;
    static unsigned *cnts = NULL;
    static size_t dcap = 0;
    static size_t dlen = 0;

    if (n == 0) return 0;          /* offset is 1 – n≥1 in normal use */

    /* Ensure we can store up to ‘n’ terms */
    if ((size_t)n > seq_len) {
        size_t need = (size_t)n;
        if (seq_cap < need) {
            size_t new_cap = seq_cap? seq_cap*2 : 8;
            while (new_cap < need) new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* Ensure divisor table can hold enough entries */
        if (dcap < need) {
            size_t new_dcap = dcap? dcap*2 : 8;
            while (new_dcap < need) new_dcap <<= 1;
            divs = (seq_t *)realloc(divs, new_dcap * sizeof(seq_t));
            cnts = (unsigned *)realloc(cnts, new_dcap * sizeof(unsigned));
            dcap = new_dcap;
        }

        /* First term */
        if (seq_len == 0) {
            seq[0] = 1;
            seq_len = 1;
        }

        /* Compute missing terms up to n */
        for (size_t i = seq_len; i < need; ++i) {
            seq_t prev = seq[i - 1];
            seq_t cand = prev + 1;

            while (1) {
                seq_t d = gcd(prev, cand);
                size_t j;
                for (j = 0; j < dlen; ++j) {
                    if (divs[j] == d) break;
                }

                if (j == dlen) {                  /* new divisor */
                    if (dlen == dcap) {           /* grow divisor table */
                        size_t new_cap = dcap? dcap*2 : 8;
                        while (new_cap <= dlen) new_cap <<= 1;
                        divs = (seq_t *)realloc(divs, new_cap * sizeof(seq_t));
                        cnts = (unsigned *)realloc(cnts, new_cap * sizeof(unsigned));
                        dcap = new_cap;
                    }
                    divs[dlen] = d;
                    cnts[dlen] = 1;
                    dlen++;
                    seq[i] = cand;
                    break;
                } else if (cnts[j] <= 1) {        /* permissible reuse */
                    cnts[j]++;
                    seq[i] = cand;
                    break;
                }
                ++cand;                           /* try next candidate */
            }
        }
        seq_len = need;
    }

    return seq[n - 1];
}
