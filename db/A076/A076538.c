/*
 * A076538  Numerators a(n) of fractions slowly converging to e: let a(1) = 0,
 *          b(n) = n - a(n); if (a(n) + 1) / b(n) < e then a(n+1) = a(n) + 1,
 *          else a(n+1)= a(n).
 * https://oeis.org/A076538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = ⌊ n * e / (e + 1) ⌋ The computation uses a high‑precision
 *            rational approximation of e with 15 decimal digits, sufficient
 *            for all n ≤ 100 000
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  e ≈ 2.71828182845904523536...
    We use P = round(e * 10^15) and D = P + 10^15.
    Then (P / D) is a lower‑bound approximation of e/(e+1),
    so floor(n * P / D) equals the required sequence value. */
#include <stdint.h>

static const uint64_t SCALE_P  = 2718281828459045ULL;   /* round(e * 1e15)   */
static const uint64_t SCALE_D  = 3718281828459045ULL;   /* SCALE_P + 1e15    */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Use unsigned 128‑bit arithmetic to avoid overflow of n * SCALE_P.
       prod ≤ 10000 * 2.7e15 < 2.8e19 < 2^128, safely fitting in __uint128_t. */
    __uint128_t prod = (__uint128_t)n * SCALE_P;
    uint64_t   q     = (uint64_t)(prod / SCALE_D);   /* floor division */

    return (seq_t)q;
}
