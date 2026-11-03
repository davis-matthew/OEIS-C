/*
 * A007653  Coefficients of L-series for elliptic curve "37a1": y^2 + y = x^3
 *          - x.
 * https://oeis.org/A007653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative with a(p) = p - #{(x,y) mod p : y^2 + y =
 *            x^3 - x}, a(p^e) = a(p)*a(p^(e-1)) - p*a(p^(e-2)) for p != 37,
 *            a(37^e) = a(37)^e, a(1) = 1, a(p^0) = 1
 * Range:     i8 n <= 941, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 8893 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static long long ap_of(uint64_t p) {
    /* cnt[r] = number of y in [0,p) with y^2 + y == r (mod p) */
    long long *cnt = (long long *)calloc((size_t)p, sizeof(long long));
    for (uint64_t y = 0; y < p; y++) {
        uint64_t r = (y * y + y) % p;
        cnt[r]++;
    }
    long long affine = 0;
    for (uint64_t x = 0; x < p; x++) {
        uint64_t x3 = (x * x % p) * x % p;
        uint64_t rhs = (x3 + p - x) % p;
        affine += cnt[rhs];
    }
    free(cnt);
    return (long long)p - affine;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t temp = n;
    uint64_t p;
    for (p = 2; p <= temp; p++) {
        if (p * p > temp) p = temp;
        if (temp % p != 0) continue;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        seq_t ap = (seq_t)ap_of(p);
        seq_t prev2 = 1;
        seq_t prev1 = ap;
        for (idx_t i = 2; i <= e; i++) {
            seq_t curr;
            if (p == 37)
                curr = ap * prev1;
            else
                curr = ap * prev1 - (seq_t)p * prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        result *= prev1;
    }
    return result;
}
