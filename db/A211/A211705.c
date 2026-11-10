/*
 * A211705  Decimal expansion of Sum_{n>=1} A006218(n)*2^(-n).
 * https://oeis.org/A211705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit of 2*Sum_{k>=1} 1/(2^k-1) = Sum_{m>=1}
 *            A000005(m)*2^(1-m), computed in binary fixed point, converted to
 *            decimal digits
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    uint64_t nn = (uint64_t)n;
    size_t L0 = (size_t)((nn * 34 / 10 + 64) / 32 + 1);
    uint64_t F = 32 * (uint64_t)L0;
    uint64_t M = F + 1;
    size_t L = L0 + 2;

    uint32_t *d = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    if (!d || !t) { free(d); free(t); return 0; }

    for (uint64_t i = 1; i <= M; i++)
        for (uint64_t j = i; j <= M; j += i) d[j]++;

    /* Horner: T = sum d(N) 2^(M-N) */
    for (uint64_t N = 1; N <= M; N++) {
        uint32_t carry = 0;
        for (size_t i = 0; i < L; i++) {
            uint32_t nc = t[i] >> 31;
            t[i] = (t[i] << 1) | carry;
            carry = nc;
        }
        uint64_t add = d[N];
        for (size_t i = 0; i < L && add; i++) {
            uint64_t s = (uint64_t)t[i] + add;
            t[i] = (uint32_t)s;
            add = s >> 32;
        }
    }
    /* value S = T / 2^(M-1) = T / 2^F ; fraction = limbs 0..L0-1 */
    unsigned digit = 0;
    for (uint64_t step = 1; step < nn; step++) {
        uint64_t carry = 0;
        for (size_t i = 0; i < L0; i++) {
            uint64_t p = (uint64_t)t[i] * 10 + carry;
            t[i] = (uint32_t)p;
            carry = p >> 32;
        }
        digit = (unsigned)carry;
    }
    free(d);
    free(t);
    return (seq_t)digit;
}
