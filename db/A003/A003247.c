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
 * Helpers:   a3247_ab, a3247_in3234, a3247_isqrt, a3247_lower
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3247_ab.h"
#include "../utils/a3247_in3234.h"
#include "../utils/a3247_isqrt.h"
#include "../utils/a3247_lower.h"

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
