/*
 * A233758  Bisection of A006950 (the even part).
 * https://oeis.org/A233758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006950(2*n-2), where sum A006950(m) x^m = Product_{k>=1}
 *            (1 + x^(2k-1)) / (1 - x^(2k)); computed by partition DP over
 *            parts k mod 4 != 2
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A233758_memo = 0;
static idx_t A233758_cap = 0;

static seq_t A006950_a(idx_t n) {
    if (n >= A233758_cap) {
        idx_t newcap = n + 1;
        if (newcap < 2 * A233758_cap) newcap = 2 * A233758_cap;
        if (newcap < 64) newcap = 64;
        seq_t *f = mem_new(seq_t, (size_t)newcap);
        for (idx_t i = 0; i < newcap; i++) f[i] = 0;
        f[0] = 1;
        for (idx_t k = 1; k < newcap; k++) {
            if (k % 4 == 2) continue;
            for (idx_t i = k; i < newcap; i++) {
                f[i] += f[i - k];
            }
        }
        if (A233758_memo) mem_free(A233758_memo);
        A233758_memo = f;
        A233758_cap = newcap;
    }
    return A233758_memo[n];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    return A006950_a(2 * (n - 1));
}
