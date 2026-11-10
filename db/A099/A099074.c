/*
 * A099074  Partial sums of A000960.
 * https://oeis.org/A099074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n+1} A000960(k), where A000960(k) starts with
 *            k^2 and successively rounds down to the next lower multiple of
 *            k-1, k-2, ..., 1 (strictly lower when already a multiple)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000960_a(idx_t n) {
    seq_t result = (seq_t)n * (seq_t)n;
    for (seq_t divisor = (seq_t)n - 1; divisor > 0; divisor--) {
        seq_t quotient = result / divisor;
        if (result % divisor == 0) {
            quotient--;
        }
        result = quotient * divisor;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n + 1; k++) {
        sum += A000960_a(k);
    }
    return sum;
}
