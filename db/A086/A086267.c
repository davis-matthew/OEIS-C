/*
 * A086267  a(n) = 3 + (H(n) mod 6) + floor(r) where H()=A005185() and r =
 *          (H(n) - 2*H(n+1) + H(n+2) - 4) / H(n).
 * https://oeis.org/A086267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, obsc
 * Formula:   a(n) = 3 + (H(n) mod 6) + floor((H(n) - 2*H(n+1) + H(n+2) -
 *            4)/H(n)), H = A005185 (Hofstadter Q: H(1)=H(2)=1,
 *            H(n)=H(n-H(n-1))+H(n-H(n-2)))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A005185_memo = NULL;
static idx_t A005185_len = 0;

static seq_t A005185_a(idx_t n) {
    if (n <= 0) return 0;
    if (n <= 2) return 1;
    if (n >= A005185_len) {
        idx_t old_len = A005185_len;
        idx_t new_len = n + 1;
        seq_t *p = mem_new(seq_t, (size_t)new_len);
        if (old_len > 0) {
            mem_copy(p, A005185_memo, (size_t)old_len * sizeof(seq_t));
            mem_free(A005185_memo);
        }
        A005185_memo = p;
        if (old_len < 3) {
            A005185_memo[1] = 1;
            A005185_memo[2] = 1;
            old_len = 3;
        }
        for (idx_t i = old_len; i <= n; i++) {
            idx_t i1 = i - (idx_t)A005185_memo[i - 1];
            idx_t i2 = i - (idx_t)A005185_memo[i - 2];
            A005185_memo[i] = A005185_memo[i1] + A005185_memo[i2];
        }
        A005185_len = new_len;
    }
    return A005185_memo[n];
}

seq_t a(idx_t n) {
    seq_t h0 = A005185_a(n);
    seq_t h1 = A005185_a(n + 1);
    seq_t h2 = A005185_a(n + 2);
    seq_t base = 3 + h0 % 6;
    seq_t P = h0 + h2;
    seq_t Q = 2 * h1 + 4;
    if (P >= Q) {
        return base + (P - Q) / h0;
    } else {
        seq_t d = Q - P;
        seq_t c = (d + h0 - 1) / h0;
        return base - c;
    }
}
