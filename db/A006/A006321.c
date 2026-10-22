/*
 * A006321  Royal paths in a lattice.
 * https://oeis.org/A006321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n+3) - 5*S(n+2) + 5*S(n+1) - S(n), where S = A006318
 *            (large Schroeder numbers), from G.f. A(x) =
 *            ((1-5x+5x^2-x^3)*S(x) - 1 + 3x - x^2)/x^3; S(0) = 1, S(m) =
 *            S(m-1) + sum_{k=0..m-1} S(k)*S(m-1-k) (fixed-width build,
 *            division-free), or (m+1)*S(m) = 3*(2m-1)*S(m-1) - (m-2)*S(m-2),
 *            S(0)=1, S...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *S = 0;
static idx_t S_len = 0;

static void ensure(idx_t m) {
    idx_t newlen, i, k;
    bool fixed;
    if (m < S_len) return;
    newlen = m + 1;
    if (newlen < 2) newlen = 2;
    S = grow_zero(S, S_len, newlen);
    fixed = seq_is_fixed_width();
    for (i = S_len; i < newlen; i++) {
        if (i == 0) { S[0] = 1; continue; }
        if (fixed) {
            seq_t s = S[i - 1];
            for (k = 0; k < i; k++) s += S[k] * S[i - 1 - k];
            S[i] = s;
        } else {
            if (i == 1) { S[1] = 2; continue; }
            S[i] = ((seq_t)3 * ((seq_t)2 * (seq_t)i - 1) * S[i - 1]
                    - ((seq_t)i - 2) * S[i - 2]) / ((seq_t)i + 1);
        }
    }
    S_len = newlen;
}

seq_t a(idx_t n) {
    ensure(n + 3);
    return S[n + 3] - (seq_t)5 * S[n + 2] + (seq_t)5 * S[n + 1] - S[n];
}
