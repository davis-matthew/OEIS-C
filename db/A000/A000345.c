/*
 * A000345  Number of partitions into non-integral powers.
 * https://oeis.org/A000345
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x1,x2,x3): 1 <= x1 <= x2 <= x3,
 *            sqrt(x1)+sqrt(x2)+sqrt(x3) <= n}. Search: for each x1 <= x2,
 *            find the largest x3 with an exact integer test (squared out in
 *            __int128) and count x3 in [x2, max]
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   a345_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/a345_ok.h"

seq_t a(idx_t n)
{
    long long N = (long long)n;
    double dn = (double)N;
    seq_t count = 0;

    for (long long x1 = 1; ; x1++) {
        if (!a345_ok(N, x1, x1, x1)) break;
        double s1 = sqrt((double)x1);
        for (long long x2 = x1; ; x2++) {
            if (!a345_ok(N, x1, x2, x2)) break;
            double t = dn - s1 - sqrt((double)x2);
            if (t < 0) t = 0;
            long long c0 = (long long)(t * t);
            if (c0 < x2) c0 = x2;
            while (a345_ok(N, x1, x2, c0 + 1)) c0++;
            while (c0 > x2 && !a345_ok(N, x1, x2, c0)) c0--;
            count += (seq_t)(uint64_t)(c0 - x2 + 1);
        }
    }
    return count;
}
