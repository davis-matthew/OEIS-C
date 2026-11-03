/*
 * A228325  a(n) is the smallest number m>n such that the concatenation nm is
 *          prime.
 * https://oeis.org/A228325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m > n such that concatenation of n and m is
 *            prime
 * Range:     u8 n <= 8, u16 n <= 866, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8415 (2026-11-01)
 * Helpers:   is_prime_seq, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* safety for offset 1 */
    uint64_t m = (uint64_t)n + 1;

    while (1) {
        uint64_t digits_m = num_digits0(m);
        uint64_t concat = (uint64_t)n * (uint64_t)pow10(digits_m) + m;
        if (is_prime_seq(concat))
            return (seq_t)m;
        ++m;
    }
}
