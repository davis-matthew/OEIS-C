/*
 * A004438  Numbers that are not the sum of 5 distinct squares.
 * https://oeis.org/A004438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   sieve -- mark every sum 0 <= a^2 < b^2 < c^2 < d^2 < e^2 (zero
 *            allowed), then a(n) is the n-th positive integer that was not
 *            marked (finite list)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define A004438_LIMIT 2000

static bool rep[A004438_LIMIT + 1];
static int rep_done = 0;

static void precompute(void) {
    int lim = A004438_LIMIT;
    for (int a = 0; a * a <= lim; a++) {
        int a2 = a * a;
        for (int b = a + 1; a2 + b * b <= lim; b++) {
            int s2 = a2 + b * b;
            for (int c = b + 1; s2 + c * c <= lim; c++) {
                int s3 = s2 + c * c;
                for (int d = c + 1; s3 + d * d <= lim; d++) {
                    int s4 = s3 + d * d;
                    for (int e = d + 1; s4 + e * e <= lim; e++) {
                        rep[s4 + e * e] = true;
                    }
                }
            }
        }
    }
    rep_done = 1;
}

seq_t a(idx_t n) {
    if (!rep_done) precompute();
    idx_t count = 0;
    for (int i = 1; i <= A004438_LIMIT; i++) {
        if (!rep[i]) {
            count++;
            if (count == n) return (seq_t)i;
        }
    }
    return (seq_t)0;
}
