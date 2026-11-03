/*
 * A166641  Totally multiplicative sequence with a(p) = 10*(p-1) for prime p.
 * https://oeis.org/A166641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e||n} (10*(p-1))^e for n ≥ 1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 8161 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Totally multiplicative: a(p) = 10*(p-1) for a prime p) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    if (n <= 1) return result;        /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;
    uint64_t p = 2;

    while (p * p <= temp) {
        if (temp % p == 0) {
            /* count exponent of prime p */
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* base = 10 * (p - 1) in seq_t */
            seq_t base = (seq_t)10 * ((seq_t)p - (seq_t)1);
            seq_t pw   = ipow_idx(base, (idx_t)exp);
            result = seq_mul(result, pw);
        }
        ++p;
    }
    /* if temp > 1, it is a prime factor with exponent 1 */
    if (temp > 1) {
        seq_t base = (seq_t)10 * ((seq_t)temp - (seq_t)1);
        result = seq_mul(result, base);   /* exp = 1 */
    }

    return result;
}
