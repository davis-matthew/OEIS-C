/*
 * A065276  The inverse permutation to A065275.
 * https://oeis.org/A065276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse of A065275: a(1)=2, a(3)=1; if n has top bits 10
 *            then 2n; if n has top bits 11 and is even then n/2; if top bits
 *            11 and odd (n>=7) then n - 2^(len(n)-2)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A065276_bitlen(idx_t x) {
    int l = 0;
    while (x) {
        x >>= 1;
        l++;
    }
    return l;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 3) return (seq_t)1;
    int len = A065276_bitlen(n);
    int second = (int)((n >> (len - 2)) & 1ULL);
    if (!second) {
        return (seq_t)n * 2;
    }
    if ((n & 1ULL) == 0) {
        return (seq_t)(n >> 1);
    }
    return (seq_t)(n - (((idx_t)1) << (len - 2)));
}
