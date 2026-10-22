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
 * Helpers:   ap_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ap_of.h"

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
