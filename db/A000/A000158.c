/*
 * A000158  Number of partitions into non-integral powers.
 * https://oeis.org/A000158
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = #{(x1,x2,x3): 1<=x1<=x2<=x3,
 *            x1^(2/3)+x2^(2/3)+x3^(2/3) <= n}. For each (x1,x2) the number of
 *            valid x3 is found with a pointer that only moves down; perfect
 *            cubes k^3 get the exact value k^2, so integer ties are decided
 *            exactly
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   a158_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/a158_isqrt.h"

static long double *a158_v = NULL;
static uint64_t a158_vn = 0;

static void a158_build(uint64_t N)
{
    uint64_t x, k;
    if (N <= a158_vn && a158_v) return;
    free(a158_v);
    a158_v = (long double *)malloc((size_t)(N + 2) * sizeof(long double));
    a158_v[0] = 0.0L;
    for (x = 1; x <= N + 1; x++)
        a158_v[x] = cbrtl((long double)x * (long double)x);
    for (k = 1; k * k * k <= N + 1; k++)
        a158_v[k * k * k] = (long double)(k * k);
    a158_vn = N;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t m, N, x1, x2, x3;
    long double ln = (long double)n;
    if (n < 3) return 0;
    m = (uint64_t)n - 2;
    N = a158_isqrt(m * m * m);   /* largest x with x^(2/3) <= n-2 */
    if (N < 1) N = 1;
    a158_build(N);

    for (x1 = 1; x1 <= N && 3.0L * a158_v[x1] <= ln; x1++) {
        long double R1 = ln - a158_v[x1];
        x3 = N;
        for (x2 = x1; x2 <= N && 2.0L * a158_v[x2] <= R1; x2++) {
            long double R2 = R1 - a158_v[x2];
            while (x3 > x2 && a158_v[x3] > R2) x3--;
            count += (seq_t)(x3 - x2 + 1);
        }
    }
    return count;
}
