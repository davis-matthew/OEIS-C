/*
 * A073150  Triangle of numbers {a(n,k), n >= 0, 0<=k<=n} defined by a(0,0)=1,
 *          a(1,0)=2, a(n,0)=A006318(n), a(n,n)=A006319(n),
 *          a(n+1,0)=a(n,0)+a(n,n), a(n,m+1)= Sum A006318(k)*a(n-k,0), k=0..m.
 * https://oeis.org/A073150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n,m) = Sum_{k=0..m} S(k)*S(n-k), where S = A006318 (large
 *            Schroeder numbers), S(0)=1, S(j) = S(j-1) + Sum_{i=0..j-1}
 *            S(i)*S(j-1-i); the sequence is the triangle read by rows, row r
 *            holds a(r,0..r)
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t m = n - r * (r + 1) / 2;

    seq_t *S = mem_new(seq_t, (size_t)(r + 2));
    S[0] = 1;
    for (idx_t j = 1; j <= r; j++) {
        seq_t t = S[j - 1];
        for (idx_t i = 0; i < j; i++) {
            t += S[i] * S[j - 1 - i];
        }
        S[j] = t;
    }

    seq_t res = 0;
    for (idx_t k = 0; k <= m; k++) {
        res += S[k] * S[r - k];
    }
    mem_free(S);
    return res;
}
