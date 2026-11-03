/*
 * A190338  Consider the iteration k -> 3*k mod 10^(number of decimal digits
 *          in n). Sequence gives the number of times the iteration has to be
 *          applied to n before returning to n.
 * https://oeis.org/A190338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the number of times the map * k → 3*k (mod 10^d) must be
 *            applied to n until returning to n, * where d is the number of
 *            decimal digits of n
 * Range:     u8 n <= 69, u16 n <= 9489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9489 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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
    /* number of decimal digits of n */
    uint64_t d = num_digits0(n);
    /* 10^d as the modulus */
    seq_t mod = pow10(d);
    /* current value in the orbit */
    seq_t k   = (seq_t)n;
    seq_t start = k;
    seq_t cnt = 0;

    do
    {
        k = (seq_t)3 * k % mod;
        cnt++;
    } while (k != start);

    return cnt;
}
