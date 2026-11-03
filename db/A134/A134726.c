/*
 * A134726  Concatenation of next n cubes of positive integers.
 * https://oeis.org/A134726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the next n cubes of positive integers
 *            where the sequence of integers runs without gaps: start(n) = 1 +
 *            (n-1)*n/2. For each k = 0 … n-1 compute cur = start(n) + k cube
 *            = cur^3 digits = number of decimal digits of cube result =
 *            result * 10^digits + cube The utilities pow10(i...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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
    if (n == 0)
        return 0;

    seq_t start = (seq_t)n * ((seq_t)n - 1) / 2 + 1;
    seq_t result = 0;

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t cur   = start + (seq_t)i;
        seq_t cube  = cur * cur * cur;
        int    digs = num_digits(cube);
        result = result * pow10((idx_t)digs) + cube;
    }
    return result;
}
