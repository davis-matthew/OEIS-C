/*
 * A132927  Concatenation of first n elements of the divisor function d(n),
 *          where d(n) is the number of divisors of n.
 * https://oeis.org/A132927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat_{k=1..n} d(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   num_digits, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(k) is the divisor count of k.
 * The result is built by concatenating the decimal representations of
 * the divisor counts of 1,2,...,n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/num_divisors.h"

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
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t d = num_divisors((uint64_t)k);
        seq_t d_seq = (seq_t)d;
        int digs = num_digits(d_seq);
        seq_t pow = pow10((idx_t)digs);
        result = result * pow + d_seq;
    }
    return result;
}
