/*
 * A098668  Right edge of triangle A098666.
 * https://oeis.org/A098668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A098666(n,n) = X(n,n-1), where X(i,0)=i,
 *            X(i,k)=X(i,k-1)/gcd(T(i-1,k),X(i,k-1)),
 *            T(i,k)=T(i-1,k)/gcd(T(i-1,k),X(i,k-1)), T(1,1)=1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    size_t sz = (size_t)n + 2;
    seq_t *prev = mem_new(seq_t, sz);
    seq_t *cur  = mem_new(seq_t, sz);
    if (!prev || !cur) {
        mem_free(prev); mem_free(cur);
        return 0;
    }
    prev[1] = (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t x = (seq_t)i;
        for (idx_t j = 1; j < i; ++j) {
            seq_t g = gcd(prev[j], x);
            cur[j] = prev[j] / g;
            x = x / g;
        }
        cur[i] = x;
        result = x;
        seq_t *t = prev; prev = cur; cur = t;
    }
    mem_free(prev);
    mem_free(cur);
    return result;
}
