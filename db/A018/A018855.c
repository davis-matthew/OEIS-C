/*
 * A018855  Smallest triangular number that begins with n.
 * https://oeis.org/A018855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   smallest triangular number that begins with n
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9929 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t pow10 = (seq_t)1;          /* 10^0 */
    for (;;)
    {
        /* Interval [L, U] where a triangular number could begin with n */
        seq_t L = n_s * pow10;
        seq_t U = (n_s + (seq_t)1) * pow10 - (seq_t)1;

        /* Compute minimal k such that T_k >= L */
        seq_t eightL      = seq_mul((seq_t)8, L);          /* 8 * L */
        seq_t sqrtInput   = eightL + (seq_t)1;            /* 1 + 8L */
        seq_t sqrtVal     = floor_sqrt(sqrtInput);        /* floor sqrt */
        seq_t k = (sqrtVal - (seq_t)1) / (seq_t)2;        /* floor of formula */

        /* Adjust k upwards until T_k >= L */
        while (true)
        {
            seq_t Tk = seq_mul(k, k + (seq_t)1) / (seq_t)2;
            if (Tk < L)
                ++k;
            else
                break;
        }

        seq_t T = seq_mul(k, k + (seq_t)1) / (seq_t)2;
        if (T <= U)
            return T;

        /* Next exponent of 10 */
        pow10 = pow10 * (seq_t)10;
    }
}
