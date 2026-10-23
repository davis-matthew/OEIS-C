/*
 * A171029  In the sequence of prime numbers, replace all the '6' digits with
 *          '1' and vice versa.
 * https://oeis.org/A171029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171029: a(n) = transform of the nth prime, swapping digits 1 ↔ 6 */
#include "../utils/get_prime.h"

static seq_t transform_prime(uint32_t p) {
    uint8_t digits[32];
    int len = 0;
    while (p > 0) {
        uint8_t d = p % 10;
        uint8_t nd;
        if (d == 1)
            nd = 6;
        else if (d == 6)
            nd = 1;
        else
            nd = d;
        digits[len++] = nd;
        p /= 10;
    }
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) {
        /* undefined for offset 1; return 0 as a safe fallback */
        return 0;
    }
    uint32_t prime = get_prime(n);  /* nth prime (1‑indexed) */
    return transform_prime(prime);
}
