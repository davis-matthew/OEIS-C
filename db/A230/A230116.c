/*
 * A230116  Value of row n in triangle A166360 when seen as binary number.
 * https://oeis.org/A230116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} T(n,k)*2^(k-1), where T(n,k) = Narayana(n,k)
 *            mod 2 = A166360(n,k); T(n,k)=1 iff v2(C(n,k)*C(n,k-1)/n) = 0,
 *            using popcount (Kummer/Legendre)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 935 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A230116_pc(uint64_t x) {
    int c = 0;
    while (x) { x &= x - 1; c++; }
    return c;
}

seq_t a(idx_t n) {
    uint64_t r = (uint64_t)n;
    int pcr = A230116_pc(r);
    int v2r = 0;
    while (((r >> v2r) & 1) == 0) v2r++;
    seq_t result = 0;
    for (uint64_t k = r; k >= 1; k--) {
        int e2 = A230116_pc(k) + A230116_pc(r - k) + A230116_pc(k - 1)
               + A230116_pc(r - k + 1) - 2 * pcr - v2r;
        result = result * 2;
        if (e2 == 0) result = result + 1;
    }
    return result;
}
