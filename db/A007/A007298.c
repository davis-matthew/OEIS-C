/*
 * A007298  Sums of consecutive Fibonacci numbers.
 * https://oeis.org/A007298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th smallest distinct value among 0 and the sums
 *            F(j)+F(j+1)+...+F(i), 1 <= j <= i (equivalently the differences
 *            F(i+2)-F(j+1)); search over the sums: every sum using F(i) with
 *            i > m is >= F(m+1), so all values below F(m+1) come from
 *            F(1..m). Increase m until there are at least n such values
 * Range:     u8 n <= 58, u16 n <= 236, u32 n <= 995, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a7298_sift, a7298_sort
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a7298_sift.h"
#include "../utils/a7298_sort.h"

#define A7298_MAXM 400

static seq_t A7298_F[A7298_MAXM + 2];
static seq_t A7298_S[A7298_MAXM * (A7298_MAXM + 1) / 2 + 2];

seq_t a(idx_t n) {
    seq_t *F = A7298_F;
    seq_t *S = A7298_S;
    F[0] = 0;
    F[1] = 1;
    F[2] = 1;
    idx_t nf = 2;
    bool wrapped = false;
    for (idx_t k = 3; k <= A7298_MAXM + 1; k++) {
        seq_t next = F[k - 1] + F[k - 2];
        if (next < F[k - 1]) { wrapped = true; break; }
        F[k] = next;
        nf = k;
    }

    for (idx_t m = 8; ; m += 8) {
        idx_t top;
        bool bounded;
        seq_t T = 0;
        bool last = false;
        if (m + 1 <= nf) {
            top = m;
            bounded = true;
            T = F[m + 1];
            if (m + 1 == nf && !wrapped) last = true;
        } else if (wrapped) {
            top = nf;
            bounded = false;
            last = true;
        } else {
            top = nf - 1;
            bounded = true;
            T = F[nf];
            last = true;
        }

        idx_t cnt = 0;
        S[cnt++] = 0;
        for (idx_t j = 1; j <= top; j++) {
            seq_t s = 0;
            for (idx_t i = j; i <= top; i++) {
                seq_t ns = s + F[i];
                if (ns < s) break;
                s = ns;
                if (bounded && !(s < T)) break;
                S[cnt++] = s;
            }
        }

        a7298_sort(S, cnt);
        idx_t k = 0;
        for (idx_t i = 0; i < cnt; i++) {
            if (k == 0 || S[i] != S[k - 1]) {
                S[k] = S[i];
                k++;
            }
        }
        if (n >= 1 && n <= k) return S[n - 1];
        if (last) return k > 0 ? S[k - 1] : (seq_t)0;
    }
}
