/*
 * A082804  Smallest multiple of 9 beginning with n.
 * https://oeis.org/A082804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n, if n ≡ 0 (mod 9); otherwise a(n) = 10·n + (9 – n mod
 *            9)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the fact that the smallest multiple of 9 that begins with n
   can be obtained by appending a single digit if required. The needed digit
   is the smallest number that brings the digit‑sum to a multiple of 9. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute digital sum modulo 9 of n. */
    uint64_t tmp = n;
    uint32_t sum_mod9 = 0;
    while (tmp > 0) {
        sum_mod9 = (sum_mod9 + (uint32_t)(tmp % 10)) % 9;
        tmp /= 10;
    }
    /* If n itself is divisible by 9, it is already the smallest multiple. */
    if (sum_mod9 == 0) {
        return (seq_t)n;
    }
    /* Otherwise we need to append a single digit that makes the sum a multiple of 9.
       That digit is d = (9 - sum_mod9) % 9, which is in 1..8. */
    uint32_t d = (9 - sum_mod9) % 9;   /* 1 <= d <= 8 */
    /* The resulting number is n followed by that digit. */
    return ((seq_t)n * (seq_t)10) + (seq_t)d;
}
