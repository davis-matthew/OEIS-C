/*
 * A003247  Complement of A003248.
 * https://oeis.org/A003247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k is in the sequence iff A003234(k) is in {A000201(A001950(x))}.
 *            A003234 = {m : frac(m*phi) > (5+sqrt(5))/10}, and
 *            A000201(A001950(x)) = 2*A000201(x) + x. The code scans both
 *            increasing sequences in step and counts matches
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

/* A000201(m) = floor(m*phi) */
static uint64_t a3247_lower(uint64_t m) {
    return (m + isqrt_u64(5 * m * m)) / 2;
}

/* A000201(A001950(x)) = A000201(x) + A001950(x) = 2*A000201(x) + x */
static uint64_t a3247_ab(uint64_t x) {
    return 2 * a3247_lower(x) + x;
}

/* m in A003234 iff frac(m*phi) > (5+sqrt5)/10,
   i.e. sqrt5*(5m-1) > 10*floor(m*phi) + 5 - 5m */
static int a3247_in3234(uint64_t m) {
    uint64_t A = a3247_lower(m);
    uint64_t l = 5 * m - 1;
    uint64_t r = 10 * A + 5 - 5 * m;
    return 5 * l * l > r * r;
}

seq_t a(idx_t n) {
    uint64_t m = 0, k = 0, cnt = 0, x = 1;
    uint64_t ab = a3247_ab(1);
    for (;;) {
        m++;
        if (!a3247_in3234(m)) continue;
        k++;
        while (ab < m) {
            x++;
            ab = a3247_ab(x);
        }
        if (ab == m) {
            cnt++;
            if (cnt == (uint64_t)n) return (seq_t)k;
        }
    }
}
