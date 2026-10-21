/*
 * A086967  Number of distinct zeros of x^5-x-1 mod prime(n).
 * https://oeis.org/A086967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A086967:
   a(n) = number of distinct zeros of x^5 - x - 1 modulo the nth prime.
   The function evaluates the polynomial for every residue 0 ≤ x < p
   where p = nth prime, counts the zeros, and returns that count.
   It performs the evaluation using only integer arithmetic. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mul_mod64.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑based). */
    uint64_t p = get_prime(n);
    if (p == 0)   /* should not happen for valid n */
        return 0;

    seq_t zeros = 0;
    for (uint64_t x = 0; x < p; ++x) {
        /* Compute x^5 mod p using repeated squaring, each multiply
           uses mul_mod64 to stay within 64‑bit arithmetic. */
        uint64_t x2 = mul_mod64(x, x, p);          /* x² */
        uint64_t x4 = mul_mod64(x2, x2, p);        /* x⁴ */
        uint64_t x5 = mul_mod64(x4, x, p);          /* x⁵ */

        /* Evaluate f(x) = x^5 - x - 1 (mod p). */
        uint64_t val = x5 + p - x + p - 1;          /* x⁵ - x - 1 (wrapped) */
        val %= p;                                   /* reduce modulo p */

        if (val == 0)
            ++zeros;
    }
    return zeros;
}
