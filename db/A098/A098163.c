/*
 * A098163  Permutation of the positive integers induced by A098162: a(n) = m
 *          iff A098162(n) - A098162(n-1) = A098162(m) - A098162(m-1) for n >
 *          1, a(1)=1.
 * https://oeis.org/A098163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m iff A098162(n)-A098162(n-1) = A098162(m)-A098162(m-1),
 *            m != n, for n>1; a(1)=1 (search over A098162 differences)
 * Range:     u8 n <= 24, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t A098162_a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static seq_t *divs = NULL;
    static unsigned *cnts = NULL;
    static size_t dcap = 0;
    static size_t dlen = 0;

    if (n == 0) return 0;

    if ((size_t)n > seq_len) {
        size_t need = (size_t)n;
        if (seq_cap < need) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 8;
            while (new_cap < need) new_cap <<= 1;
            seq = mem_resize(seq_t, seq, new_cap);
            seq_cap = new_cap;
        }
        if (dcap < need) {
            size_t new_dcap = dcap ? dcap * 2 : 8;
            while (new_dcap < need) new_dcap <<= 1;
            divs = mem_resize(seq_t, divs, new_dcap);
            cnts = (unsigned *)realloc(cnts, new_dcap * sizeof(unsigned));
            dcap = new_dcap;
        }
        if (seq_len == 0) {
            seq[0] = 1;
            seq_len = 1;
        }
        for (size_t i = seq_len; i < need; ++i) {
            seq_t prev = seq[i - 1];
            seq_t cand = prev + 1;
            while (1) {
                seq_t d = gcd(prev, cand);
                size_t j;
                for (j = 0; j < dlen; ++j) {
                    if (divs[j] == d) break;
                }
                if (j == dlen) {
                    if (dlen == dcap) {
                        size_t new_cap = dcap ? dcap * 2 : 8;
                        while (new_cap <= dlen) new_cap <<= 1;
                        divs = mem_resize(seq_t, divs, new_cap);
                        cnts = (unsigned *)realloc(cnts, new_cap * sizeof(unsigned));
                        dcap = new_cap;
                    }
                    divs[dlen] = d;
                    cnts[dlen] = 1;
                    dlen++;
                    seq[i] = cand;
                    break;
                } else if (cnts[j] <= 1) {
                    cnts[j]++;
                    seq[i] = cand;
                    break;
                }
                ++cand;
            }
        }
        seq_len = need;
    }
    return seq[n - 1];
}

static seq_t A098163_diff(idx_t m) {
    return A098162_a(m) - A098162_a(m - 1);
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    seq_t d = A098163_diff(n);
    for (idx_t m = 2; ; ++m) {
        if (m == n) continue;
        if (A098163_diff(m) == d) return (seq_t)m;
    }
}
