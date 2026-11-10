/*
 * A084251  G.f.: A(x) = exp(sum(n>=1, A084250(n)*x^n/n)), where A084250 lists
 *          the least distinct positive integers that allow A(x) to be an
 *          integer power series.
 * https://oeis.org/A084251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x)=exp(Sum_{k>=1} c(k)*x^k/k) with c=A084250 (c(1)=1, c(k)
 *            least unused positive integer making all coefficients integers);
 *            a(n)=[x^n]A(x), via n*a(n)=Sum_{k=1..n} c(k)*a(n-k), a(0)=1
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *c_seq = NULL;   /* A084250(1..len) */
    static seq_t *b_seq = NULL;   /* b(0..len) = this sequence */
    static size_t cap = 0;
    static size_t len = 0;

    if (c_seq == NULL) {
        cap = 64;
        c_seq = mem_new(seq_t, cap);
        b_seq = mem_new(seq_t, cap);
        c_seq[1] = (seq_t)1;
        b_seq[0] = (seq_t)1;
        b_seq[1] = (seq_t)1;
        len = 1;
    }

    while (len < (size_t)n) {
        size_t m = len + 1;
        seq_t sm = (seq_t)m;
        seq_t S = (seq_t)0;
        seq_t S_mod = (seq_t)0;
        for (size_t k = 1; k < m; ++k) {
            S += c_seq[k] * b_seq[m - k];
            S_mod = (S_mod + (c_seq[k] % sm) * (b_seq[m - k] % sm)) % sm;
        }
        seq_t r = (sm - S_mod) % sm;
        seq_t cand = (r == (seq_t)0) ? sm : r;
        bool used;
        do {
            used = false;
            for (size_t i = 1; i <= len; ++i) {
                if (c_seq[i] == cand) { used = true; break; }
            }
            if (used) cand += sm;
        } while (used);

        seq_t b_new = (S + cand) / sm;

        if (m + 1 >= cap) {
            cap *= 2;
            c_seq = mem_resize(seq_t, c_seq, cap);
            b_seq = mem_resize(seq_t, b_seq, cap);
        }
        c_seq[m] = cand;
        b_seq[m] = b_new;
        len = m;
    }
    return b_seq[(size_t)n];
}
