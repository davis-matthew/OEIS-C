/*
 * A032610  Concatenation of n and n + 5 or {n,n+5}.
 * https://oeis.org/A032610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+5) = n*10^{digits(n+5)} + (n+5)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    idx_t m = n + 5;
    int len = num_digits0((uint64_t)m);
    seq_t pow = pow10((idx_t)len);
    seq_t res = (seq_t)n * pow + (seq_t)m;
    return res;
}
