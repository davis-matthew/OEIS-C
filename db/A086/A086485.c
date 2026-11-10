/*
 * A086485  Duplicate of A068310.
 * https://oeis.org/A086485
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = squarefree part of n^2-1 = (n-1)*(n+1) (computed by
 *            trial-dividing n-1 and n+1)
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 24 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t m1 = (uint64_t)n - 1;
    uint64_t m2 = (uint64_t)n + 1;
    seq_t res = 1;
    for (uint64_t p = 2; p * p <= m1 || p * p <= m2; p++) {
        int e = 0;
        while (m1 % p == 0) { m1 /= p; e++; }
        while (m2 % p == 0) { m2 /= p; e++; }
        if (e & 1) res *= (seq_t)p;
    }
    /* Remaining cofactors are 1 or distinct primes. */
    if (m1 > 1) res *= (seq_t)m1;
    if (m2 > 1) res *= (seq_t)m2;
    return res;
}
