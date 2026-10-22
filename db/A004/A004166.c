/*
 * A004166  Sum of digits of 3^n.
 * https://oeis.org/A004166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(3^n), the digit sum of 3^n; 3^n is computed
 *            exactly in base 10^9 limbs
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* number of decimal digits of 3^n is about 0.4772*n + 1 */
    size_t cap = (size_t)(n / 18) + 4;
    uint32_t *limb = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 1;
    idx_t rem = n;
    unsigned long long s = 0;
    size_t i;
    seq_t r;

    limb[0] = 1;
    while (rem > 0) {
        uint64_t mul = 1;
        uint64_t carry = 0;
        int k = 0;
        while (rem > 0 && k < 19) {
            mul *= 3;
            rem--;
            k++;
        }
        for (i = 0; i < len; i++) {
            uint64_t t = (uint64_t)limb[i] * mul + carry;
            limb[i] = (uint32_t)(t % 1000000000ULL);
            carry = t / 1000000000ULL;
        }
        while (carry > 0) {
            if (len == cap) {
                cap *= 2;
                limb = (uint32_t *)realloc(limb, cap * sizeof(uint32_t));
            }
            limb[len++] = (uint32_t)(carry % 1000000000ULL);
            carry /= 1000000000ULL;
        }
    }

    for (i = 0; i < len; i++) {
        uint32_t v = limb[i];
        while (v > 0) {
            s += v % 10;
            v /= 10;
        }
    }
    free(limb);

    r = (seq_t)s;
    return r;
}
