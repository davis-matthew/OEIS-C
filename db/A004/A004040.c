/*
 * A004040  Inversion of A000257.
 * https://oeis.org/A004040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = ((1-8x)^(3/2) - 1 + 14x - 14x^2 - 2x^3) /
 *            (2x(1+x)^3), so a(n) = c(n) - 3*a(n-1) - 3*a(n-2) - a(n-3), with
 *            c(0)=1, c(1)=5, c(2)=15, c(n) = 16*A000257(n-1) =
 *            2^(n+1)*(4*C(n-1) - C(n)) for n >= 3, C = Catalan numbers (a(-1)
 *            = a(-2) = a(-3) = 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;
    static seq_t *Cat = NULL;
    static seq_t pw = 0;
    static idx_t have = 0, cap = 0;
    bool fixed;
    idx_t k;

    if (n < have) return A[n];
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        A = grow_zero(A, cap, nc);
        Cat = grow_zero(Cat, cap, nc);
        cap = nc;
    }
    fixed = seq_is_fixed_width();
    for (k = have; k <= n; k++) {
        seq_t c, s;
        if (k == 0) {
            Cat[0] = 1;
            pw = 2;
        } else if (fixed) {
            idx_t i;
            s = 0;
            for (i = 0; i < k; i++) s += Cat[i] * Cat[k - 1 - i];
            Cat[k] = s;
        } else {
            Cat[k] = Cat[k - 1] * (seq_t)(4 * k - 2) / (seq_t)(k + 1);
        }

        if (k == 0) c = 1;
        else if (k == 1) c = 5;
        else if (k == 2) c = 15;
        else c = pw * (Cat[k - 1] * 4 - Cat[k]);

        s = 0;
        if (k >= 1) s += A[k - 1] * 3;
        if (k >= 2) s += A[k - 2] * 3;
        if (k >= 3) s += A[k - 3];
        A[k] = c - s;

        pw *= 2;
    }
    have = n + 1;
    return A[n];
}
