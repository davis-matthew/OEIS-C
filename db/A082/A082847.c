/*
 * A082847  Partial sums of A082846.
 * https://oeis.org/A082847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} b(i), where b = A082846: b(1)=1, b(2)=2,
 *            b(3)=3, b(b(1)+...+b(k)) = b(k), and every other b(m) cycles
 *            1->2->3->1 from b(m-1). Computed by direct simulation
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *B = NULL;   /* B[i] = A082846(i) */
    static seq_t *S = NULL;   /* S[i] = A082847(i) */
    static idx_t len = 0;
    static idx_t cap = 0;
    static idx_t nk = 1;

    if (n < 1) return (seq_t)0;

    if (n >= cap) {
        idx_t nc = (cap == 0) ? 8 : cap * 2;
        while (nc <= n) nc *= 2;
        B = mem_resize(seq_t, B, (size_t)nc + 1);
        S = mem_resize(seq_t, S, (size_t)nc + 1);
        cap = nc;
    }

    if (len == 0) {
        B[1] = (seq_t)1; B[2] = (seq_t)2; B[3] = (seq_t)3;
        S[1] = (seq_t)1; S[2] = (seq_t)3; S[3] = (seq_t)6;
        len = 3;
        nk = 1;
    }

    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t target = (seq_t)i;
        while (nk <= len && S[nk] < target) ++nk;
        seq_t val;
        if (nk <= len && S[nk] == target) {
            val = B[nk];
        } else {
            seq_t prev = B[i - 1];
            if (prev == (seq_t)1) val = (seq_t)2;
            else if (prev == (seq_t)2) val = (seq_t)3;
            else val = (seq_t)1;
        }
        B[i] = val;
        S[i] = S[i - 1] + val;
        len = i;
    }

    return S[n];
}
