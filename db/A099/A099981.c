/*
 * A099981  Bisection of A001358.
 * https://oeis.org/A099981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001358(2n+2); computed by search, counting semiprimes in
 *            order
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int A001358_count_pf(seq_t num) {
    int count = 0;
    seq_t temp = num;
    while (temp % 2 == 0) {
        count++;
        temp /= 2;
        if (count > 2) return count;
    }
    for (seq_t d = 3; d * d <= temp; d += 2) {
        while (temp % d == 0) {
            count++;
            temp /= d;
            if (count > 2) return count;
        }
    }
    if (temp > 1) count++;
    return count;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n * 2 + 2;
    seq_t found = 0;
    for (seq_t candidate = 4; ; candidate++) {
        if (A001358_count_pf(candidate) == 2) {
            found++;
            if (found == target) return candidate;
        }
    }
}
